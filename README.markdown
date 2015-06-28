Rapport du devoir maison de structures de données à rendre pour le 22 avril 2013
Mael Valais - L2 info - mael.valais@univ-tlse3.fr

(UTF8 - markdown) 

# Instructions de test
Pour tester tout ça, faites simplement
	make

puis 
	./a.out -d dot -r etape

et enfin
	./dot_to_gif.sh -d dot etape

et _animation.gif_ pourra être ouvert !

# Ecriture de dot\_to\_gif.sh
J'ai écrit ce script pour me simplifier la vie lors de la visualisation. Il utilise _dot_ et _convert_.
Il traduit les .dot en un unique .gif en effectuant un petit réajustement des noeuds pour que ça le fasse bien.
Les réajustements sont effectués par un script écrit par Emden R. Gansner.
Je l'ai donné à toute la promo sur le groupe facebook, histoire que tout le monde puisse s'en sortir.

Désolé pour le poids des .gif résultants, je n'ai pas réussi à les compresser...

# Ecriture de rbtreeInsert
Pas grand chose à dire, je me suis un peu inspiré de l'implémentation de Todd Miller (http://www.opensource.apple.com/source/sudo/sudo-46/src/redblack.h) concernant le type _enum color_. Je trouvais ça sympa, alors je l'ai pris.

# Ecriture de rbTreeRemove (suppression simple)
Premier challenge, pour X raisons j'ai pas mal buté. En réétudiant cas par cas, j'ai identifié les soucis, et dans la plupart des cas il s'agissait des coutures. 

# Ecriture de rbSolveUnbalancedTree (correction de l'arbre après suppression)
Traduire l'algorithme naturel du cours a été le plus difficile. J'ai commencé par ajouter un _doubleblack_ dans mon _enum Color_. Le soucis posé était que dès qu'un noeud était _doubleblack_, impossible de le détecter comme un _black_ normal.

Je suis donc passé par un simple bouléen _isdoubleblack_ qui traduit l'état de _replace_.

Ah oui, le passage des bons noeuds de rbTreeRemove à rbSolveUnbalancedTree était aussi assez embêtante... Au final j'ai choisi de passer le noeud remplacé (_replace_, on l'appelait X dans le cours) et _replacefather_ car _replace_ pouvait être une sentinnelle (et donc on aurait pas pu accéder à son père et continuer l'algorithme...).

Je me suis aussi posé la question des sentinnelles modifiées : cet algorithme, à plusieurs endroits, peut modifier la sentinnelle _tree->nil_. Or, si sa couleur est modifiée, l'arbre n'est plus correct car ses feuilles peuvent être rouges... Donc à chaque fois qu'on est sur le point de sortir de _rbSolveUnbalancedTree_, l'algo répare la sentinnelle _tree->nil_.

# Conclusion
Un DM qui m'aura pris bien 8 heures, en comptant aussi la découverte de Dot, Convert et tous ces outils. J'ai dû aussi approfondir mes connaissances de GDB pour arriver à debugger vers la fin. 
J'ai en outre installé CGDB, une _text user interface_ sous Ncurses assez proche du fonctionnement de VI (dont je suis fervent utilisateur).

MV
