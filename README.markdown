L'arbre de recherche RBTree (arbre rouge-noir)
============================================
Ce projet est l'un des devoirs de l'UE *structures de données* de l'université Toulouse III — Paul Sabatier. Je remercie tout particulièrement le professeur Mathias Paulin pour ses excellents enseignements.

L'objectif est de coder puis de visualiser les différentes étapes de rééquilibrage lors d'insertions et de suppressions de noeuds d'un arbre binaire de recherche de type *rouge-noir*.

![Animation du comportement d'un arbre de recherche rouge-noir](https://raw.githubusercontent.com/maelvalais/comportement-arbre-rouge-noir-avec-dot/master/exemple_animation.gif)

## Instructions de compilation et d'exécution de `a.out`
Pour tester tout ça, faites simplement
	
	make
	./a.out -d dot -r etape
	./dot_to_gif.sh -d dot etape

et `animation.gif` pourra être ouvert ! Un exemple de ce que ça donne est affiché en haut de cette page.

## Ecriture de `dot_to_gif.sh`
J'ai écrit ce script pour me simplifier la vie lors de la visualisation. Il utilise `dot` et `convert`. Il traduit les .dot en un unique .gif en effectuant un petit réajustement des noeuds pour que ça le fasse bien. Les réajustements sont effectués par un script écrit par [Emden R. Gansner (stackoverflow)](http://stackoverflow.com/questions/10902745/enforcing-horizontal-node-ordering-in-a-dot-tree).

Je l'ai donné à toute la promo sur le groupe facebook, histoire que tout le monde puisse s'en sortir.

Désolé pour le poids des `.gif` résultants, je n'ai pas réussi à les compresser...

## Écriture de `rbtreeInsert()`
Pas grand chose à dire, je me suis un peu inspiré de l'[implémentation de Todd Miller](http://www.opensource.apple.com/source/sudo/sudo-46/src/redblack.h) concernant le type `enum color`. Je trouvais ça sympa, alors je l'ai pris.

## Écriture de `rbTreeRemove()` (suppression simple)
Premier challenge, pour X raisons j'ai pas mal buté. En réétudiant cas par cas, j'ai identifié les soucis, et dans la plupart des cas il s'agissait des coutures. 

## Écriture de `rbSolveUnbalancedTree()` (correction de l'arbre après suppression)
Traduire l'algorithme naturel du cours a été le plus difficile. J'ai commencé par ajouter un `doubleblack` dans mon `enum Color`. Le soucis posé était que dès qu'un noeud était `doubleblack`, impossible de le détecter comme un `black` normal.

Je suis donc passé par un simple bouléen `isdoubleblack` qui traduit l'état de `replace`.

Ah oui, le passage des bons noeuds de `rbTreeRemove()` à `rbSolveUnbalancedTree()` était aussi assez embêtante... Au final j'ai choisi de passer le noeud remplacé (`replace`, on l'appelait X dans le cours) et `replacefather` car `replace` pouvait être une sentinelle (et donc on aurait pas pu accéder à son père et continuer l'algorithme...).

Je me suis aussi posé la question des sentinnelles modifiées : cet algorithme, à plusieurs endroits, peut modifier la sentinnelle `tree->nil`. Or, si sa couleur est modifiée, l'arbre n'est plus correct car ses feuilles peuvent être rouges... Donc à chaque fois qu'on est sur le point de sortir de `rbSolveUnbalancedTree`, l'algo répare la sentinelle `tree->nil`.

## Conclusion
Un DM qui m'aura pris bien 8 heures, en comptant aussi la découverte de `dot`, `convert` et tous ces outils. J'ai dû aussi approfondir mes connaissances de`gdb` pour arriver à debugger vers la fin. 
J'ai en outre installé `cgdb`, une `text user interface` sous *Ncurses* assez proche du fonctionnement de vim.


----------


Maël Valais - L2 info - mael.valais@univ-tlse3.fr — devoir de structures de données à rendre pour le 22 avril 2013 — UTF8, markdown
