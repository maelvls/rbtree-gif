# Visualization of a red-black tree data structure

This project aims at visualizing the different rebalancing steps that
happen during insertions and deletions in a binary search tree of red-black
type. The [rbtree.c](/rbtree.c) contains a simple version of the red-black
tree data structure, and [main.c](/main.c) is the program that does
insertions and deletions step-by-step and then outputs a series of
[dot](https://graphviz.org/doc/info/lang.html) files which can then be
converted into a nice GIF.

This project is the result of a home assignment I had for the course "Data
Structures" during my second year of computer science at Université de
Toulouse III - Paul Sabatier. A special thank you to Professor Mathias
Paulin for his excellent teachings.

![Animation of the behavior of a red-black search tree](https://cdn.rawgit.com/maelvalais/comportement-arbre-rouge-noir-avec-dot/a5affb42/exemple_animation.gif)

## Instructions for compiling and running a.out

To test all of this, simply do


make
./a.out -d point -r "step_"
./dot_to_gif.sh $ (find dot -name "* .dot" | sort -n -t_ -k2)
and animation.gif can be opened! An example of this is displayed at the top of this page.

Writing dot_to_gif.sh
I wrote this script to make my life easier when viewing. He uses dot and convert. It translates the .dot into a single .gif by making a small readjustment of the nodes so that it does it well. Readjustments are made by a script written by [Emden R. Gansner(stackoverflow)](http://stackoverflow.com/questions/10902745/enforcing-horizontal-node-ordering-in-a-dot-tree).

I gave it to all the promo on the facebook group, so everyone can get away with it.

Sorry for the weight of the resulting .gif, I failed to compress them ...

Writing rbtreeInsert ()
Not much to say, I'm a bit inspired by [Todd Miller's implementation](http://www.opensource.apple.com/source/sudo/sudo-46/src/redblack.h) of the enum color type. I thought it was cool, so I took it.

Writing rbTreeRemove () (single delete)
First challenge, for X reasons I stumbled quite a bit. By re-studying case by case, I identified the concerns, and in most cases there are seams.

Writing rbSolveUnbalancedTree () (tree fix after deletion)
Translating the course's natural algorithm was the most difficult. I started by adding a double black in my Color enum. The concern was that as soon as a node was doubleblack, it was impossible to detect it like a normal black.

So I went through a simple isdoubleblack bouléen which reflects the replacement state.

Ah yes, the passage of the good nodes from rbTreeRemove () to rbSolveUnbalancedTree () was also quite annoying ... In the end I chose to pass the replaced node (replace, we called it X in the course) and replacefather because replace could be a sentinel (and therefore we could have accessed his father and continued the algorithm ...).

I also asked myself the question of modified sentinals: this algorithm, in several places, can modify the tree-> nil sentinal. Or, if its color is changed, the tree is no longer correct because its leaves may be red ... So every time we are about to exit rbSolveUnbalancedTree, the algo repairs the tree-> sentinel nothing.

Conclusion
A DM that took me 8 hours, also counting the discovery of dowry, converts and all these tools. I also had to deepen my degdb knowledge to get to debugger towards the end. I also installed cgdb, a text user interface under Ncurses quite similar to how vim works.

Maël Valais <mael.valais@gmail.com>