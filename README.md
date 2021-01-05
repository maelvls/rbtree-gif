# Visualization of a red-black tree data structure

This project aims at visualizing the different rebalancing steps that
happen during insertions and deletions in a binary search tree of red-black
type. The [rbtree.c](/rbtree.c) contains a simple version of the red-black
tree data structure, and [main.c](/main.c) is the program that does
insertions and deletions step-by-step and then outputs a series of
[dot](https://graphviz.org/doc/info/lang.html) files which can then be
converted into a nice gif using `dot_to_gif.sh`.

`dot_to_gif.sh` was initially meant to help me debug my red-tree
rebalancing algorithm. It uses `dot` (from
[graphviz](https://graphviz.org/download/)) and `convert` (from
[imagemagick](https://imagemagick.org/script/download.php)). The script
translates a series of `.dot` files into a single `.gif` by making a
~~small~~ readjustment of the nodes so that it looks good. Readjustments
are made by a script written by [Emden R.
Gansner(stackoverflow)](http://stackoverflow.com/questions/10902745/enforcing-horizontal-node-ordering-in-a-dot-tree).


This project is the result of a home assignment I had for the course "Data
Structures" during my second year of computer science at Université de
Toulouse III - Paul Sabatier. A special thank you to Professor Mathias
Paulin for his excellent teachings.

![Animation of the behavior of a red-black search tree][gif]

## Instructions for generating the gif

To test all of this, do:

```sh
sudo apt install graphviz # Linux
brew install graphviz     # macOS

make
./rbtree_to_dot -d point -r "step_"
./dot_to_gif.sh $ (find dot -name "* .dot" | sort -n -t_ -k2)
```

The file `animation.gif` will get generated (similar as to the one above)
can be opened! An example of it is displayed at the top of this readme.

[gif]: https://cdn.rawgit.com/maelvalais/comportement-arbre-rouge-noir-avec-dot/a5affb42/exemple_animation.gif
