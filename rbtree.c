/*
 * queue.c
 *
 * A red-black tree data structure stored in a flat array. A few average
 * time complexities, with n being the number of elements stored:
 *
 * algorithm   avg. complexity  worst scenario
 * ===========================================
 * insert      O(log n)         O(log n)
 * search      O(log n)         O(log n)
 * delete      O(log n)         O(log n)
 *
 * Copyright (C) 2013-2020  Mael Valais
 */

#include "rbtree.h"

#include <stdlib.h>

#include "assert.h"
#include "key.h"

enum color
{
	red,
	black
};

struct node
{
	void *key;
	struct node *left;
	struct node *right;
	struct node *father;
	enum color color;
};

struct rbtree
{
	int (*cmp)(const void *, const void *);
	int (*equal)(const void *, const void *);
	struct node *root;
	struct node *nil; /* sentinelle, initialisée noire */
};

void rbtree_solve_unbalanced_tree(struct rbtree *tree, struct node *replace, struct node *replace_father);

struct rbtree *rbtree_new(int (*cmp)(const void *, const void *), int (*equal)(const void *, const void *))
{
	struct rbtree *tree = (struct rbtree *)malloc(sizeof(struct rbtree));
	tree->cmp = cmp;
	tree->equal = equal;

	/* the nil */
	tree->nil = (struct node *)malloc(sizeof(struct node));
	tree->nil->left = tree->nil->right = tree->nil->father = tree->nil;
	tree->nil->color = black;
	tree->nil->key = NULL;

	/* the fake root */
	tree->root = (struct node *)malloc(sizeof(struct node));
	tree->root->left = tree->root->right = tree->root->father = tree->nil;
	tree->root->color = black;
	tree->root->key = NULL;

	return (tree);
}

bool rbtree_empty(struct rbtree *tree)
{
	return tree->root->left == tree->nil;
}

void rbtree_rotate_right(struct rbtree *tree, struct node *n)
{
	struct node *m = n->left;

	n->left = m->right;	   /* on bouge le noeud qui bouge de gauche à droite (B) */
	m->father = n->father; /* on modifie aussi son père */

	if (n->left != tree->nil) /* on remet le bon pere à B */
		n->left->father = n;

	if (n->father->left == n) /* le lien vers le fils de la racine est modifie */
		n->father->left = m;
	else
		n->father->right = m;
	m->right = n; /* inversement des liens pere-fils */
	n->father = m;
	//	printf("Rotation droite sur %d\n",keyPut(n->key)); /* XXX DEBUG */
}

void rbtree_rotate_left(struct rbtree *tree, struct node *n)
{
	struct node *m = n->right;

	n->right = m->left; /* on s'occupe de B qui change de pere */
	m->father = n->father;

	if (n->right != tree->nil) /* on modif le pere de B aussi */
		n->right->father = n;

	if (n->father->left == n) /* on modifie la racine */
		n->father->left = m;
	else
		n->father->right = m;

	m->left = n; /* on modif le lien etre m et n */
	n->father = m;
	//	printf("Rotation gauche sur %d\n",keyPut(n->key)); /* XXX DEBUG */
}

struct node *_classic_tree_insert(struct rbtree *tree, struct node *node, struct node *added, struct node *father)
{
	if (node == tree->nil)
	{
		added->father = father;
		node = added;
		return (added);
	}
	else
	{
		if ((*tree->cmp)(added->key, node->key))
			node->left = _classic_tree_insert(tree, node->left, added, node);
		else
			node->right = _classic_tree_insert(tree, node->right, added, node);
		return (node);
	}
}

void rbtree_classic_tree_insert(struct rbtree *tree, struct node *added)
{
	if (rbtree_empty(tree))
		tree->root->left = _classic_tree_insert(tree, tree->root->left, added, tree->root);
	else
		_classic_tree_insert(tree, tree->root->left, added, tree->root);
}

void rbtree_insert(struct rbtree *tree, void *data)
{
	/* ============ Phase 1 : ajout classique ============ */

	struct node *added = (struct node *)malloc(sizeof(struct node));
	added->key = data;
	added->left = added->right = tree->nil;
	added->color = red;
	rbtree_classic_tree_insert(tree, added);

	struct node *uncle;
	/* ============ Phase 2 : gestion des clashs ============ */
	while (added->father->color == red)
	{ /* on remonte deux generations à chaque fois */
		if (added->father == added->father->father->left)
		{ /* ==== CAS GAUCHE ==== */
			uncle = rb_grandparent(added)->right;
			/* ============ Cas 1 : l'oncle est rouge, on recolorie ============ */
			if (uncle->color == red)
			{
				added->father->color = uncle->color = black;
				rb_grandparent(added)->color = red;
				added = rb_grandparent(added); /* on remonte de deux generations car c'est clean */
			}
			else
			{
				/* ============ Cas 3 : l'oncle est noir, on revient sur Cas 2 ============ */
				if (added == added->father->right)
				{
					added = added->father;
					rbtree_rotate_left(tree, added); /* added est de nouveau petit fils */
				}
				/* ============ Cas 2 : l'oncle est noir, coloriage+rota============ */
				added->father->color = black; /* on echange les couleurs */
				rb_grandparent(added)->color = red;
				rbtree_rotate_right(tree, rb_grandparent(added));
			}
		}
		else
		{ /* ==== CAS DROIT ==== */
			uncle = rb_grandparent(added)->left;
			if (uncle->color == red)
			{
				added->father->color = uncle->color = black;
				rb_grandparent(added)->color = red;
				added = rb_grandparent(added); /* on remonte de deux generations car c'est clean */
			}
			else
			{
				if (added == added->father->left)
				{
					added = added->father;
					rbtree_rotate_right(tree, added);
				}
				added->father->color = black;
				rb_grandparent(added)->color = red;
				rbtree_rotate_left(tree, rb_grandparent(added));
			}
		}
	}
	rb_first(tree)->color = black;
}

void rbtree_to_dot(struct rbtree *tree, const char *racine, const char *dossier)
{
	assert(!rbtree_empty(tree));

	static int numerofichier = 0;
	char final[30];
	sprintf(final, "%s/%s%d.dot", dossier, racine, numerofichier++);
	FILE *fd = fopen(final, "wt");

	fprintf(fd, "digraph G { \n");

	struct node *node;
	struct queue *queue;
	queue_new(&queue);
	queue_add(queue, rb_first(tree));
	do
	{
		node = queue_remove(queue);

		if (node->color == red)
			fprintf(fd, "\t%d [color=red];\n", key_put(node->key));
		else
			fprintf(fd, "\t%d [color=black];\n", key_put(node->key));

		if (node->left != tree->nil)
		{
			fprintf(fd, "\t%d -> %d;\n", key_put(node->key), key_put(node->left->key));
			if (node->left->color == red)
				fprintf(fd, "\t%d [color=red];\n", key_put(node->left->key));
			else
				fprintf(fd, "\t%d [color=black];\n", key_put(node->left->key));
		}
		if (node->right != tree->nil)
		{
			fprintf(fd, "\t%d -> %d;\n", key_put(node->key), key_put(node->right->key));
			if (node->right->color == red)
				fprintf(fd, "\t%d [color=red];\n", key_put(node->right->key));
			else
				fprintf(fd, "\t%d [color=black];\n", key_put(node->right->key));
		}

		if (node->left != tree->nil)
			queue_add(queue, node->left);
		if (node->right != tree->nil)
			queue_add(queue, node->right);

	} while (!queue_empty(queue));

	fprintf(fd, "}\n");
	fclose(fd);
}

void rbtree_map_debug(struct rbtree *tree)
{
	assert(!rbtree_empty(tree));
	struct node *node;
	struct queue *queue;
	queue_new(&queue);
	queue_add(queue, rb_first(tree));
	printf("\033[01;35m==== Début de l'arbre ====\n\033[0m");
	do
	{
		node = queue_remove(queue);
		if (rb_exists(node->left) || rb_exists(node->right) || rb_first(tree) == node)
		{
			if (node->father != tree->root)
				printf("(pere: %d)", key_put(node->father->key));
			if (node->color == red)
				printf("\033[01;31m");
			if (rb_first(tree) == node)
				printf("\033[01;32m");
			printf("Noeud %d\033[0m", key_put(node->key));

			if (rb_exists(node->left))
			{
				if (node->left->color == red)
					printf("\033[01;31m");
				printf(", ");
				if (node->left->father != tree->root)
					printf("(pere: %d) ", key_put(node->left->father->key));
				printf("%d fils gauche\033[0m", key_put(node->left->key));
			}
			if (rb_exists(node->right))
			{
				printf(", ");
				if (node->right->color == red)
					printf("\033[01;31m");
				if (node->right->father != tree->root)
					printf("(pere: %d) ", key_put(node->right->father->key));
				printf("%d fils droit\033[0m", key_put(node->right->key));
			}
			printf("\n");
		}

		if (node->left != tree->nil)
			queue_add(queue, node->left);
		if (node->right != tree->nil)
			queue_add(queue, node->right);

	} while (!queue_empty(queue));
	printf("\n");
}

/* retourne si le noeud supprime etait rouge ET le noeud remplacant*/
void rbtree_remove(struct rbtree *tree, void *data)
{
	struct node *replace, *replace_father;
	struct node *node = rb_first(tree);
	while (!(*tree->equal)(node->key, data))
	{
		node = (*tree->cmp)(node->key, data) ? node->right : node->left;
	}
	/* node est le noeud à supprimer */
	if (rb_is_leaf(node))
	{
		if (node->father->right == node)
			node->father->right = tree->nil;
		else
			node->father->left = tree->nil;
		replace_father = node->father;
		replace = tree->nil;
	}
	else
	{ /* Ce noeud n'est pas une feuille */
		if (node->left == tree->nil)
		{ /* unique noeud à droite */		 /* FIXME */
			if (node == node->father->right) /* droit en premier pour le cas de la racine */
				node->father->right = node->right;
			else
				node->father->left = node->right;
			if (rb_exists(node->right))
				node->right->father = node->father; /* XXX AJOUTE */
			replace_father = node->father;
			replace = node->right;
		}
		else if (node->right == tree->nil)
		{									 /* unique noeud à gauche */
			if (node == node->father->right) /* droit en premier pour le cas de la racine */
				node->father->right = node->left;
			else
				node->father->left = node->left;
			if (rb_exists(node->left))
				node->left->father = node->father; /* XXX AJOUTE */
			replace_father = node->father;
			replace = node->left;
		}
		else
		{ /* ==== deux noeuds  ==== */
			struct node *temp = node->right;
			while (temp->left != tree->nil) /* une fois à droite puis tout à gauche */
				temp = temp->left;
			node->key = temp->key;
			if (temp->father->left == temp)
			{
				temp->father->left = temp->right; /* on raccroche l'hypothetique fils droit de temp */
				if (rb_exists(temp->right))
					temp->right->father = temp->father;
			}
			else
			{
				temp->father->right = temp->right; /* si temp est juste à droite de node, on raccroche */
				if (rb_exists(temp->right))
					temp->right->father = temp->father;
			}
			replace_father = temp->father;
			replace = temp->right;
			node = temp; /* histoire d'avoir le meme node que dans le reste du code */
		}
	}
	/* (x) node est le noeud qui a ete supprime, il devra etre free() XXX */
	/* (y_father) node->father == replace_father == replace->father (à tous les coups) */
	/* (y) replace est le noeud qui remplace celui qui a ete supprime */
	replace_father = node->father;

	if (node->color == black)
	{
		if (replace->color == red)
			replace->color = black;
		else
			rbtree_solve_unbalanced_tree(tree, replace, replace_father);
	}
	free(node);
}

void swap_colors(struct node *a, struct node *b)
{
	enum color temp = a->color;
	a->color = b->color;
	b->color = temp;
}
void mend_sentinels(struct rbtree *tree)
{
	tree->nil->color = black;
	tree->root->color = black;
}
void add_black(struct node *a, int *isdoubleblack)
{
	if (a->color == red)
	{
		a->color = black;
		*isdoubleblack = 0;
	}
	else
		*isdoubleblack = 1;
}

void rbtree_solve_unbalanced_tree(struct rbtree *tree, struct node *replace, struct node *replace_father)
{
	/* Soient :
	 * y : replace, le noeud remplacé
	 * p : replace_father, le pere du noeud remplacé
	 * f : frere de y
	 * g : fils gauche de f
	 * d : fils droit de f
	 * */
	int isdoubleblack = 1; /* etat de replace */
	while (replace != rb_first(tree) && isdoubleblack)
	{ /* on s'arretera à la racine */

		if (replace == replace_father->left)
		{ /* CAS GAUCHE */
			if (replace_father->right->color == black)
			{ /* f est noir */
				if (replace_father->right->right->color == black && replace_father->right->left->color == black)
				{ /* CAS 1.A */ /*(g et d sont noirs)*/
					//printf("Cas 1.A gauche\n");
					replace->color = black;					   /* y devient simple noir */
					replace_father->right->color = red;		   /* f devient rouge */
					add_black(replace_father, &isdoubleblack); /* p devient double noir */

					replace = replace_father; /* y devient p */
					replace_father = replace->father;
				}
				else if (replace_father->right->right->color == red)
				{ /* CAS 1.B */
					//printf("Cas 1.B gauche\n");
					swap_colors(replace_father, replace_father->right); /* f prend la couleur de p */
					replace_father->right->right->color = black;		/* d devient noir */
					replace_father->color = black;						/* p devient noir */
					rbtree_rotate_left(tree, replace_father);			/* rotation gauche en p */
					isdoubleblack = 0;									/* y devient noir */
					mend_sentinels(tree);
					return;
				}
				else if (replace_father->right->left->color == red && replace_father->right->right->color == black)
				{ /* CAS 1.C */
					//printf("Cas 1.C gauche\n"); /* f est noir, g est rouge, d est noir */
					swap_colors(replace_father->right->left, replace_father->right); /* g devient noir, f rouge */
					rbtree_rotate_right(tree, replace_father->right);				 /* rotation droite en f */
																					 /* la prochaine boucle retournera sur 1.B */
				}
			}
			else
			{ /* f est rouge */
				//printf("Cas 2 gauche\n");
				swap_colors(replace_father, replace_father->right); /* on echange les couleurs de p et f */
				rbtree_rotate_left(tree, replace_father);			/* rotation gauche en p */
																	/* on revient au cas 1 */
			}
		}

		else
		{ /* CAS DROIT */
			if (replace_father->left->color == black)
			{ /* f est noir */
				if (replace_father->left->left->color == black && replace_father->left->right->color == black)
				{ /* CAS 1.A */ /*(g et d sont noirs)*/
					//printf("Cas 1.A droit\n");
					replace->color = black;					   /* y devient simple noir */
					replace_father->left->color = red;		   /* f devient rouge */
					add_black(replace_father, &isdoubleblack); /* p devient double noir */

					replace = replace_father; /* y devient p */
					replace_father = replace->father;
				}
				else if (replace_father->left->left->color == red)
				{ /* CAS 1.B */
					//printf("Cas 1.B droit\n");
					swap_colors(replace_father, replace_father->left); /* f prend la couleur de p */
					replace_father->left->left->color = black;		   /* d devient noir */
					replace_father->color = black;					   /* p devient noir */
					rbtree_rotate_right(tree, replace_father);		   /* rotation gauche en p */
					isdoubleblack = 0;								   /* y devient noir */
					mend_sentinels(tree);
					return;
				}
				else if (replace_father->left->right->color == red && replace_father->left->left->color == black)
				{ /* CAS 1.C */
					//printf("Cas 1.C droit\n"); /* f est noir, g est rouge, d est noir */
					swap_colors(replace_father->left->right, replace_father->left); /* g devient noir, f rouge */
					rbtree_rotate_left(tree, replace_father->left);					/* rotation droite en f */

					/* la prochaine boucle retournera sur 1.B */
				}
			}
			else
			{ /* f est rouge */
				//printf("Cas 2 droit\n");
				swap_colors(replace_father, replace_father->left); /* on echange les couleurs de p et f */
				rbtree_rotate_right(tree, replace_father);		   /* rotation gauche en p */
																   /* on revient au cas 1 en ne changeant rien */
			}
		}
	}
	mend_sentinels(tree);
}

/* \033[01;31m 	couleur : rouge */
/* \033[01;35m	couleur : mauve */
/* \033[01;32m	couleur : vert */
/* \033[01;34m	couleur : bleu */
/* \033[0m		raz de la couleur */
