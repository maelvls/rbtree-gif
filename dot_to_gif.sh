#! /bin/bash
#
# dottogif.sh
# Copyright (C) 2013 Mael Valais <mael.valais@univ-tlse3.fr>
#
#

IN=etape #.dot
OUT=animation.gif
TEMP=tempimage
FILETYPE=gif

DOT_DIR=.

TIME=100 # en millisecondes

OPT_KEEP_PIC=0 # 0 = non
OPT_OPEN=0 # ouvre avec open le gif 
OPT_RM_ROOT_FILES=0 # supprime les IN


SCRIPT=script_tree_joli.dot

LOOP=0 # si ça joue en boucle ou pas (0=oui)

if ! which dot > /dev/null; then
	echo "dot n'est pas installe... Installez le paquet 'graphviz''" 2>&1
	exit 1
fi
if ! which convert > /dev/null; then
	echo "convert n'est pas installe... Installez le paquet 'ImageMagick'"
	exit 1
fi
if [ ! -f "$SCRIPT" ]; then
	echo "Le script $SCRIPT ne semble pas present"
	exit 1
fi

if [ $# -eq 0 ]; then
	echo "Usage: $0 [-k][-c][-o] racine (voir --help)" 2>&1
	exit 1
fi
while [ "$1" ]; do
	case "$1" in
		"--keep" | "-k")
			OPT_KEEP_PIC=1;;
		"--open" | "-o")
			OPT_OPEN=1;;
		"-c"| "--clean")
			OPT_RM_ROOT_FILES=1;;
		"--help"|"-h")
			echo "`basename $0` : transformer une serie de .dot en un gif anime"
			echo "==== Options : ===="
			echo "   -k pour garder les images temporaires"
			echo "   -c pour supprimer les fichiers d'entree (contenant la racine)"
			echo "   -o pour ouvrir le gif directement apres avec la commande open"
			echo " 	 -d pour modifier le chemin de travail du script (fichiers ${IN}.dot et ${TEMP}.${FILETYPE})"
			echo "==== Pour 'racine' : ===="
			echo "   C'est en fait la racine des fichier .dot qu'on veut modeliser"
			echo "   Par exemple, si les fichiers sont fichierDot0.dot, fichierDot1.dot..."
			echo "   alors on appelera : $0 fichierDot"
			echo "   ATTENTION : le premier fichier .dot doit contenir 0 !"
			echo "	 La racine par defaut est $IN"
			echo "Auteur : Mael Valais - mael.valais@univ-tlse3.fr"
			exit 0
			;;
		"-d" ) # ou chercher les .dot
			if [ "$2" ]; then
				DOT_DIR="$2"
				shift
			else 
				echo "Erreur: l'option -d prend un nom de dossier (-h pour l'aide)" 2>&1
				exit 1
			fi
			;;
		*)
			IN="$1";;
	esac
	shift
done

if [ ! -d "$DOT_DIR" ]; then
	echo "Erreur: le dossier $DOT_DIR n'existe pas (-h pour l'aide)" 2>&1
	exit 1
fi


if [ ! "$IN" ]; then
	echo "Usage: $0 [-k][-c][-o] racine (voir --help)" 2>&1
	exit 1
fi

if ! ls $DOT_DIR | grep "$IN" > /dev/null; then
	echo "La racine $IN n'est contenue dans aucun fichier ! (-h pour l'aide)" 2>&1
	echo "Le premier fichier doit etre ${IN}0.dot" 2>&1
	exit 2
fi

test=`ls $DOT_DIR | grep "$IN" | while read a; do echo $a | grep -v "\.dot"; done`
if [ "$test" ]; then
	echo "Erreur: le fichier $test comprenant cette racine est erroné (-h pour l'aide)" 2>&1
	exit 2
fi

cpt=0
size_max_x=0
size_max_y=0
fichier=${DOT_DIR}/${IN}${cpt}.dot
while [ -f "$fichier" ]; do
	if echo $fichier | grep -v '\.dot' > /dev/null; then
		echo "$fichier n'est pas un fichier .dot correct" 2>&1
		exit 3
	fi
	echo "Traitement de $fichier"
	dot "$fichier" | gvpr -c -f${SCRIPT} 2> /dev/null | neato -n -Tpng -o ${DOT_DIR}/${TEMP}${cpt}.${FILETYPE}
	size_temp=`identify "${DOT_DIR}/${TEMP}${cpt}.${FILETYPE}" | cut -d " " -f 3`
	size_temp_x=`echo $size_temp | cut -d 'x' -f1`
	size_temp_y=`echo $size_temp | cut -d 'x' -f2`

	if [ "$size_temp_x" -gt "$size_max_x" ]; then size_max_x=$size_temp_x; fi
	if [ "$size_temp_y" -gt "$size_max_y" ]; then size_max_y=$size_temp_y; fi
	
	(( cpt++ ))
	fichier=${DOT_DIR}/${IN}${cpt}.dot
done

liste=""
for (( i = 0; i < cpt; i++ )); do
	liste="${liste} ${DOT_DIR}/${TEMP}${i}.${FILETYPE}"
done

echo "Creation de ${OUT} (taille ${size_max_x}x${size_max_y})..."

convert -delay ${TIME} -loop ${LOOP} -extent ${size_max_x}x${size_max_y} -dispose Background -background white $liste ${OUT}
(( size_max_x=$size_max_x-90 ))
((size_max_y=$size_max_y-12))
convert -pointsize 14 -fill red -draw "text $size_max_x,$size_max_y 'par Mael V'" ${OUT} ${OUT} 

if [ $OPT_OPEN -eq 1 ]; then
	open ${OUT}
fi

if [ $OPT_KEEP_PIC -eq 0 ]; then
	rm ${liste}
fi

if [ $OPT_RM_ROOT_FILES -eq 1 ]; then
	rm ${DOT_DIR}/${IN}*
fi

