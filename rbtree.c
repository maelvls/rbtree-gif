/* ===========================================================
 *
 *       Filename:  rbtree.c
 *
 *    Description:
 *
 *        Created:  03-04-2013 14:14
 *
 *         Author:  Mael Valais
 *         Mail  :	mael.valais@univ-tlse3.fr
 *
 * =========================================================== */

#include "rbtree.h"
#include <stdio.h>
#include <assert.h>
#include "key.h"

typedef enum _color {
	red,
	black
} Color;

struct _node {
	void *key;
	struct _node *left;
	struct _node *right;
	struct _node *father;
	Color color;
};

struct _rbtree {
	int (*cmp)(const void*, const void*);
	int (*equal)(const void*, const void*);
	struct _node *root;
	struct _node *nil; /* sentinelle, initialisée noire */
};

void rbSolveUnbalancedTree(RBTree tree, Node replace, Node replacefather);


RBTree rbtreeCreate(int (*cmp)(const void*, const void*), int (*equal)(const void*, const void*)){
	RBTree tree = (RBTree)malloc(sizeof(struct _rbtree));
	tree->cmp = cmp;
	tree->equal = equal;

	/* ============ création du nil ============ */
	tree->nil = (Node)malloc(sizeof(struct _node));
	tree->nil->left = tree->nil->right = tree->nil->father = tree->nil;
	tree->nil->color = black;
	tree->nil->key = NULL;

	/* ============ création du root factice ============ */
	tree->root = (Node)malloc(sizeof(struct _node));
	tree->root->left = tree->root->right = tree->root->father = tree->nil;
	tree->root->color = black;
	tree->root->key = NULL;

	return(tree);
}

int rbtreeEmpty(RBTree tree) {
	return tree->root->left == tree->nil;
}

void rbtreeRotateRight(RBTree tree, Node n) {
	Node m = n->left;

	n->left = m->right; /* on bouge le noeud qui bouge de gauche à droite (B) */
	m->father = n->father; /* on modifie aussi son père */

	if(n->left != tree->nil) /* on remet le bon pere à B */
		n->left->father = n;

	if(n->father->left == n) /* le lien vers le fils de la racine est modifie */
		n->father->left = m;
	else
		n->father->right = m;
	m->right = n; /* inversement des liens pere-fils */
	n->father = m;
//	printf("Rotation droite sur %d\n",keyPut(n->key)); /* XXX DEBUG */
}

void rbtreeRotateLeft(RBTree tree, Node n) {
	Node m = n->right;

	n->right = m->left; /* on s'occupe de B qui change de pere */
	m->father = n->father;

	if(n->right != tree->nil) /* on modif le pere de B aussi */
		n->right->father = n;

	if(n->father->left == n) /* on modifie la racine */
		n->father->left = m;
	else
		n->father->right = m;

	m->left = n;	/* on modif le lien etre m et n */
	n->father = m;
//	printf("Rotation gauche sur %d\n",keyPut(n->key)); /* XXX DEBUG */
}


Node intern_classicTreeInsert(RBTree tree, Node node, Node added, Node father) {
	if(node == tree->nil) {
		added->father = father;
		node = added;
		return(added);
	}
	else {
		if((*tree->cmp)(added->key,node->key))
			node->left = intern_classicTreeInsert(tree,node->left,added,node);
		else
			node->right = intern_classicTreeInsert(tree,node->right,added,node);
		return(node);
	}
}

void rbtreeClassicTreeInsert(RBTree tree, Node added) {
	if(rbtreeEmpty(tree))
		tree->root->left = intern_classicTreeInsert(tree,tree->root->left,added,tree->root);
	else
		intern_classicTreeInsert(tree,tree->root->left,added,tree->root);
}


void rbtreeInsert(RBTree tree, void *data) {
	/* ============ Phase 1 : ajout classique ============ */

	Node added = (Node)malloc(sizeof(struct _node));
	added->key = data;
	added->left = added->right = tree->nil;
	added->color = red;
	rbtreeClassicTreeInsert(tree, added);

	Node uncle;
	/* ============ Phase 2 : gestion des clashs ============ */
	while(added->father->color == red) { /* on remonte deux generations à chaque fois */
		if(added->father == added->father->father->left) { /* ==== CAS GAUCHE ==== */
			uncle = rbgrandpa(added)->right;
			/* ============ Cas 1 : l'oncle est rouge, on recolorie ============ */
			if(uncle->color == red) {
				added->father->color = uncle->color = black;
				rbgrandpa(added)->color = red;
				added = rbgrandpa(added); /* on remonte de deux generations car c'est clean */
			}
			else {
				/* ============ Cas 3 : l'oncle est noir, on revient sur Cas 2 ============ */
				if(added == added->father->right) {
					added = added->father;
					rbtreeRotateLeft(tree,added); /* added est de nouveau petit fils */
				}
				/* ============ Cas 2 : l'oncle est noir, coloriage+rota============ */
				added->father->color = black; /* on echange les couleurs */
				rbgrandpa(added)->color = red;
				rbtreeRotateRight(tree,rbgrandpa(added));
			}
		}
		else { /* ==== CAS DROIT ==== */
			uncle = rbgrandpa(added)->left;
			if(uncle->color == red) {
				added->father->color = uncle->color = black;
				rbgrandpa(added)->color = red;
				added = rbgrandpa(added); /* on remonte de deux generations car c'est clean */
			}
			else {
				if(added == added->father->left) {
					added = added->father;
					rbtreeRotateRight(tree,added);
				}
				added->father->color = black;
				rbgrandpa(added)->color = red;
				rbtreeRotateLeft(tree,rbgrandpa(added));
			}
		}
	}
	rbfirst(tree)->color = black;
}


void rbtreeToDot(RBTree tree, const char* racine, const char* dossier) {
	assert(!rbtreeEmpty(tree));

	static int numerofichier=0;
	char final[30];
	sprintf(final,"%s/%s%d.dot",dossier,racine,numerofichier++);
	FILE*fd = fopen(final,"wt");

	fprintf(fd,"digraph G { \n");

	Node node;
	QUEUE queue;
	queueCreate(&queue);
	queueAdd(queue,rbfirst(tree));
	do {
		node = queueRemove(queue);

		if(node->color==red)
			fprintf(fd,"\t%d [color=red];\n",keyPut(node->key));
		else
			fprintf(fd,"\t%d [color=black];\n",keyPut(node->key));

		if(node->left != tree->nil) {
			fprintf(fd,"\t%d -> %d;\n",keyPut(node->key),keyPut(node->left->key));
			if(node->left->color==red)
				fprintf(fd,"\t%d [color=red];\n",keyPut(node->left->key));
			else
				fprintf(fd,"\t%d [color=black];\n",keyPut(node->left->key));
		}
		if(node->right != tree->nil) {
			fprintf(fd,"\t%d -> %d;\n",keyPut(node->key),keyPut(node->right->key));
			if(node->right->color==red)
				fprintf(fd,"\t%d [color=red];\n",keyPut(node->right->key));
			else
				fprintf(fd,"\t%d [color=black];\n",keyPut(node->right->key));
		}


		if(node->left != tree->nil)
			queueAdd(queue,node->left);
		if(node->right != tree->nil)
			queueAdd(queue,node->right);

	} while(!queueEmpty(queue));

	fprintf(fd,"}\n");
	fclose(fd);
}

void rbtreeMapDebug(RBTree tree) {
	assert(!rbtreeEmpty(tree));
	Node node;
	QUEUE queue;
	queueCreate(&queue);
	queueAdd(queue,rbfirst(tree));
	printf("\033[01;35m==== Début de l'arbre ====\n\033[0m");
	do {
		node = queueRemove(queue);
		if(rbExists(node->left) || rbExists(node->right) || rbfirst(tree)==node) {
			if(node->father != tree->root) printf("(pere: %d)",keyPut(node->father->key));
			if(node->color==red) printf("\033[01;31m");
			if(rbfirst(tree)==node) printf("\033[01;32m");
			printf("Noeud %d\033[0m",keyPut(node->key));

			if(rbExists(node->left))  {
				if(node->left->color==red) printf("\033[01;31m");
				printf(", ");
				if(node->left->father != tree->root) printf("(pere: %d) ",keyPut(node->left->father->key));
				printf("%d fils gauche\033[0m",keyPut(node->left->key));
			}
			if(rbExists(node->right))  {
				printf(", ");
				if(node->right->color==red) printf("\033[01;31m");
				if(node->right->father != tree->root) printf("(pere: %d) ",keyPut(node->right->father->key));
				printf("%d fils droit\033[0m",keyPut(node->right->key));
			}
		printf("\n");
	}

		if(node->left != tree->nil)
			queueAdd(queue,node->left);
		if(node->right != tree->nil)
			queueAdd(queue,node->right);

	} while(!queueEmpty(queue));
	printf("\n");
}


/* retourne si le noeud supprime etait rouge ET le noeud remplacant*/
void rbtreeRemove(RBTree tree, void* data) {
	Node replace, replacefather;
	Node node = rbfirst(tree);
	while(!(*tree->equal)(node->key,data)) {
		node = (*tree->cmp)(node->key,data)?node->right:node->left;
	}
	/* node est le noeud à supprimer */
	if(rbIsLeaf(node)) {
		if(node->father->right == node)
			node->father->right = tree->nil;
		else
			node->father->left = tree->nil;
		replacefather = node->father;
		replace = tree->nil;
	}
	else { /* Ce noeud n'est pas une feuille */
		if(node->left == tree->nil) { /* unique noeud à droite */ /* FIXME */
			if(node == node->father->right) /* droit en premier pour le cas de la racine */
				node->father->right = node->right;
			else
				node->father->left = node->right;
			if(rbExists(node->right))
				node->right->father = node->father;	/* XXX AJOUTE */
			replacefather = node->father;
			replace = node->right;
		}
		else if(node->right == tree->nil) { /* unique noeud à gauche */
			if(node == node->father->right) /* droit en premier pour le cas de la racine */
				node->father->right = node->left;
			else
				node->father->left = node->left;
			if(rbExists(node->left))
				node->left->father = node->father;	/* XXX AJOUTE */
			replacefather = node->father;
			replace = node->left;
		}
		else { /* ==== deux noeuds  ==== */
			Node temp = node->right;
			while(temp->left != tree->nil) /* une fois à droite puis tout à gauche */
				temp = temp->left;
			node->key = temp->key;
			if(temp->father->left == temp) {
				temp->father->left = temp->right; /* on raccroche l'hypothetique fils droit de temp */
				if(rbExists(temp->right)) temp->right->father = temp->father;
			}
			else {
				temp->father->right = temp->right; /* si temp est juste à droite de node, on raccroche */
				if(rbExists(temp->right)) temp->right->father = temp->father;
			}
			replacefather = temp->father;
			replace = temp->right;
			node = temp; /* histoire d'avoir le meme node que dans le reste du code */
		}
	}
	/* (x) node est le noeud qui a ete supprime, il devra etre free() XXX */
	/* (y_father) node->father == replacefather == replace->father (à tous les coups) */
	/* (y) replace est le noeud qui remplace celui qui a ete supprime */
	replacefather = node->father;

	if(node->color == black) {
		if(replace->color == red)
			replace->color = black;
		else
			rbSolveUnbalancedTree(tree,replace,replacefather);
	}
	free(node);
}

void swapColors(Node a, Node b) {
	Color temp = a->color;
	a->color = b->color;
	b->color = temp;
}
void mendSentinels(RBTree tree){
	tree->nil->color = black;
	tree->root->color = black;
}
void addBlack(Node a, int *isdoubleblack){
	if(a->color==red) {
		a->color = black;
		*isdoubleblack = 0;
	}
	else
		*isdoubleblack = 1;
}

void rbSolveUnbalancedTree(RBTree tree, Node replace, Node replacefather) {
	/* Soient :
	 * y : replace, le noeud remplacé
	 * p : replacefather, le pere du noeud remplacé
	 * f : frere de y
	 * g : fils gauche de f
	 * d : fils droit de f
	 * */
	int isdoubleblack = 1; /* etat de replace */
	while(replace != rbfirst(tree) && isdoubleblack) { /* on s'arretera à la racine */

		if(replace == replacefather->left) { /* CAS GAUCHE */
			if(replacefather->right->color == black) { /* f est noir */
				if(replacefather->right->right->color == black && replacefather->right->left->color == black) {/* CAS 1.A */ /*(g et d sont noirs)*/
					//printf("Cas 1.A gauche\n");
					replace->color = black; /* y devient simple noir */
					replacefather->right->color = red; /* f devient rouge */
					addBlack(replacefather,&isdoubleblack); /* p devient double noir */

					replace = replacefather; /* y devient p */
					replacefather = replace->father;
				}
				else if(replacefather->right->right->color == red) { /* CAS 1.B */
					//printf("Cas 1.B gauche\n");
					swapColors(replacefather,replacefather->right); /* f prend la couleur de p */
					replacefather->right->right->color = black; /* d devient noir */
					replacefather->color = black; /* p devient noir */
					rbtreeRotateLeft(tree,replacefather); /* rotation gauche en p */
					isdoubleblack = 0; /* y devient noir */
					mendSentinels(tree);
					return;
				}
				else if(replacefather->right->left->color == red && replacefather->right->right->color == black) { /* CAS 1.C */
					//printf("Cas 1.C gauche\n"); /* f est noir, g est rouge, d est noir */
					swapColors(replacefather->right->left,replacefather->right);/* g devient noir, f rouge */
					rbtreeRotateRight(tree,replacefather->right); /* rotation droite en f */
					/* la prochaine boucle retournera sur 1.B */
				}
			}
			else { /* f est rouge */
				//printf("Cas 2 gauche\n");
				swapColors(replacefather,replacefather->right); /* on echange les couleurs de p et f */
				rbtreeRotateLeft(tree,replacefather); /* rotation gauche en p */
				/* on revient au cas 1 */
			}
		}

		else { /* CAS DROIT */
			if(replacefather->left->color == black) { /* f est noir */
				if(replacefather->left->left->color == black && replacefather->left->right->color == black) {/* CAS 1.A */ /*(g et d sont noirs)*/
					//printf("Cas 1.A droit\n");
					replace->color = black; /* y devient simple noir */
					replacefather->left->color = red; /* f devient rouge */
					addBlack(replacefather,&isdoubleblack); /* p devient double noir */

					replace = replacefather; /* y devient p */
					replacefather = replace->father;
				}
				else if(replacefather->left->left->color == red) { /* CAS 1.B */
					//printf("Cas 1.B droit\n");
					swapColors(replacefather,replacefather->left); /* f prend la couleur de p */
					replacefather->left->left->color = black; /* d devient noir */
					replacefather->color = black; /* p devient noir */
					rbtreeRotateRight(tree,replacefather); /* rotation gauche en p */
					isdoubleblack = 0; /* y devient noir */
					mendSentinels(tree);
					return;
				}
				else if(replacefather->left->right->color == red && replacefather->left->left->color == black) { /* CAS 1.C */
					//printf("Cas 1.C droit\n"); /* f est noir, g est rouge, d est noir */
					swapColors(replacefather->left->right,replacefather->left);/* g devient noir, f rouge */
					rbtreeRotateLeft(tree,replacefather->left); /* rotation droite en f */

					/* la prochaine boucle retournera sur 1.B */
				}
			}
			else { /* f est rouge */
				//printf("Cas 2 droit\n");
				swapColors(replacefather,replacefather->left); /* on echange les couleurs de p et f */
				rbtreeRotateRight(tree,replacefather); /* rotation gauche en p */
				/* on revient au cas 1 en ne changeant rien */
			}
		}
	}
	mendSentinels(tree);
}



	/* \033[01;31m 	couleur : rouge */
	/* \033[01;35m	couleur : mauve */
	/* \033[01;32m	couleur : vert */
	/* \033[01;34m	couleur : bleu */
	/* \033[0m		raz de la couleur */

