#! /usr/bin/env bash
#
# (MIT license)
# Copyright 2013 Mael Valais <mael.valais@gmail.fr>
#
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation
# the rights to use, copy, modify, merge, publish, distribute, sublicense,
# and/or sell copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
# DEALINGS IN THE SOFTWARE.


OUT=animation.gif
TIME=100 # in milliseconds
OPT_RM_INTERMEDIATES=1 # 0 = no
WATERMARK=""
LOOP=0 # The resulting GIF should loop (0=no)

function help() {
cat <<EOF
Usage: $(basename $0) INPUT [INPUT...] [-o FILE] [-t TIMEMS] [options]

Description;
    $(basename $0) allows you to create a GIF using a set of DOT files.

Options:
    INPUT      Files in dot format used for creating the GIF. The order
               of the INPUT arguments will be used for the order of images
               in the GIF.
    -o FILE    Give a different name to the ouput GIF [default: $OUT]
    -t MS      Set the time (in milliseconds) between two images in
               the GIF [default: ${TIME}].
    -w TEXT    Add some text on bottom-right of the GIF [default: $WATERMARK]
    -k         Keep the intermediate PNG files
    -l         Enable loop playing of the produced GIF

Author:
    Mael Valais <mael.valais@gmail.com>
EOF
}

if ! which dot > /dev/null; then
cat <<EOF >&2
Error: 'dot' not found, Graphviz is probably not installed.
To install it on macOS:
    brew install graphviz
On ubuntu:
    sudo apt-get install graphviz
EOF
	exit 1
fi
if ! which convert > /dev/null; then
cat <<EOF >&2
Error: 'convert' not found, ImageMagick is probably not installed.
To install it on macOS:
    brew install imagemagick
On ubuntu:
    sudo apt-get install imagemagick
EOF
	exit 1
fi

if [ $# -eq 0 ]; then
	help | head -1 >&2
	exit 1
fi

# Nb of dot files
nb_inputs=0
inputs=

while [ "$1" ]; do
	case "$1" in
		"--keep" | "-k")
			OPT_RM_INTERMEDIATES=1;;
		"--help"|"-h")
			help
			;;
		"-l")
			LOOP=1
			;;
		"-o" ) # Name of the gif file
			if [ "$2" ]; then
				OUT="$2"
				shift
			else
				echo "Error: -o needs a file path" >&1
				help | head -1 >&2
				exit 1
			fi
			;;
		"-t" ) # Time (in ms) between images
			if [ "$2" ]; then
				TIME="$2"
				shift
			else
				echo "Error: -t needs an argument" >&1
				help | head -1 >&2
				exit 1
			fi
			;;
		"-w" ) # Watermark in GIF
			if [ "$2" ]; then
				WATERMARK="$2"
				shift
			else
				echo "Error: -w needs an argument" >&1
				help | head -1 >&2
				exit 1
			fi
			;;
		-? | --*)
			echo "Error: unknown flag $1" >&2
			exit 1
			;;
		*)
			(( nb_inputs++ ))
			inputs[$nb_inputs]="$1"
			intermediates[$nb_inputs]="$1.png"
	esac
	shift
done

if [ $nb_inputs -eq 0 ]; then
	echo "Error: missing FILE [FILE...]" >&2
	help | head -1 >&2
	exit 1
fi

cat <<"EOF" > /tmp/script.dot
// script par Emden R. Gansner
// trouve sur http://stackoverflow.com/questions/10902745/enforcing-horizontal-node-ordering-in-a-dot-tree

BEGIN {
  double tw[node_t];    // width of tree rooted at node
  double nw[node_t];    // width of node
  double xoff[node_t];  // x offset of root from left side of its tree
  double sp = 36;       // extra space between left and right subtrees
  double wd, w, w1, w2;
  double x, y, z;
  edge_t e1, e2;
  node_t n;
}
BEG_G {
  $.bb = "";
  $tvtype=TV_postfwd;   // visit root after all children visited
}
N {
  sscanf ($.width, "%f", &w);
  w *= 72;  // convert inches to points
  nw[$] = w;
  if ($.outdegree == 0) {
    tw[$] = w;
    xoff[$] = w/2.0;
  }
  else if ($.outdegree == 1) {
    e1 = fstout($);
    w1 = tw[e1.head];
    tw[$] = w1 + (sp+w)/2.0;
    if (e1.side == "left")
      xoff[$] = tw[$] - w/2.0;
    else
      xoff[$] = w/2.0;
  }
  else {
    e1 = fstout($);
    w1 = tw[e1.head];
    e2 = nxtout(e1);
    w2 = tw[e2.head];
    wd = w1 + w2 + sp;
    if (w > wd)
      wd = w;
    tw[$] = wd;
    xoff[$] = w1 + sp/2.0;
  }
}
BEG_G {
  $tvtype=TV_fwd;   // visit root first, then children
}
N {
  if ($.indegree == 0) {
    sscanf ($.pos, "%f,%f", &x, &y);
    $.pos = sprintf("0,%f", y);
  }
  if ($.outdegree == 0) return;
  sscanf ($.pos, "%f,%f", &x, &y);
  wd = tw[$];
  e1 = fstout($);
  n = e1.head;
  sscanf (n.pos, "%f,%f", &z, &y);
  if ($.outdegree == 1) {
    if (e1.side == "left")
      n.pos = sprintf("%f,%f",  x - tw[n] - sp/2.0 + xoff[n], y);
    else
      n.pos = sprintf("%f,%f", x + sp/2.0 + xoff[n], y);
  }
  else {
    n.pos = sprintf("%f,%f", x - tw[n] - sp/2.0 + xoff[n], y);
    e2 = nxtout(e1);
    n = e2.head;
    sscanf (n.pos, "%f,%f", &z, &y);
    n.pos = sprintf("%f,%f", x + sp/2.0 + xoff[n], y);
  }
}
EOF

cpt=0
size_max_x=0
size_max_y=0

for fichier in ${inputs[@]}; do
	echo "Processing $fichier"
	dot "$fichier" | gvpr -c -f/tmp/script.dot | neato -n -Tpng -o "${fichier}.png"
	size_temp=`identify "${fichier}.png" | cut -d " " -f 3`
	size_temp_x=`echo $size_temp | cut -d 'x' -f1`
	size_temp_y=`echo $size_temp | cut -d 'x' -f2`

	if [ "$size_temp_x" -gt "$size_max_x" ]; then size_max_x=$size_temp_x; fi
	if [ "$size_temp_y" -gt "$size_max_y" ]; then size_max_y=$size_temp_y; fi
done

echo "Generation of ${OUT} (size: ${size_max_x}x${size_max_y})"

convert -delay ${TIME} -loop ${LOOP} -extent ${size_max_x}x${size_max_y} -dispose Background -background white ${intermediates[@]} ${OUT}
(( size_max_x=$size_max_x-90 ))
((size_max_y=$size_max_y-12))
convert -pointsize 14 -fill red -draw "text $size_max_x,$size_max_y '${WATERMARK}'" ${OUT} ${OUT}

if [ $OPT_RM_INTERMEDIATES -eq 1 ]; then
	rm ${intermediates[@]}
fi
