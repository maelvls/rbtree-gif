/* ===========================================================
 *
 *       Filename:  main.c
 *
 *    Description:
 *
 *        Created:  06-04-2013 22:39
 *
 *         Author:  Mael Valais
 *         Mail  :	mael.valais@univ-tlse3.fr
 *
 * =========================================================== */

#include "key.h"
#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <dirent.h>

#define DOSSIER_DOT "dot"
#define RACINE_DOT "step_"

#define VERBOSE 1

int main(int argc, const char *argv[])
{
	char dossier[30] = DOSSIER_DOT;
	char racine[30] = RACINE_DOT;
	int i, mode = 0;
	DIR *d;

	/* ==== traitement des parametres ==== */
	for (i = 1; i < argc; ++i)
	{
		if (argv[i][0] != '-')
		{
			strcpy(dossier, argv[i]); /* c'est le nom du dossier */
		}
		if (0 == strcmp(argv[i], "-h") || 0 == strcmp(argv[i], "--help"))
		{
			printf("Usage: %s [-h][dossier][-r racine] \n", argv[0]);
			printf("Par defaut, le dossier est '%s'\n", DOSSIER_DOT);
			printf("-h ou --help pour l'aide\n");
			exit(0);
		}
		if (strcmp(argv[i], "-r") == 0)
		{
			if (i + 1 < argc)
			{
				strcpy(racine, argv[i + 1]);
			}
			else
			{
				fprintf(stderr, "Erreur: l'option -r doit suivre d'un nom de racine (-h pour l'aide)\n");
				exit(1);
			}
			i++;
		}
		if (strcmp(argv[i], "-v") == 0)
		{
			mode = mode | VERBOSE;
		}
	}

	/* ==== verif des parametres ==== */
	if ((d = opendir(dossier)))
	{
		closedir(d);
	}
	else
	{
		fprintf(stderr, "Erreur: le dossier '%s' n'existe pas - veuillez le creer (-h pour l'aide)\n", dossier);
		exit(1);
	}

	struct rbtree *tree = rbtree_create(key_cmp, key_equal);
	int N = 0;
	int *tab[100];

	tab[N++] = key_create(18);
	tab[N++] = key_create(0);
	tab[N++] = key_create(19);
	tab[N++] = key_create(23);
	tab[N++] = key_create(2);
	tab[N++] = key_create(24);
	tab[N++] = key_create(10);
	tab[N++] = key_create(3);
	tab[N++] = key_create(11);
	tab[N++] = key_create(8);
	tab[N++] = key_create(12);
	tab[N++] = key_create(13);
	tab[N++] = key_create(7);
	tab[N++] = key_create(14);
	tab[N++] = key_create(20);
	tab[N++] = key_create(4);
	tab[N++] = key_create(21);
	tab[N++] = key_create(5);
	tab[N++] = key_create(22);
	tab[N++] = key_create(6);
	tab[N++] = key_create(15);
	tab[N++] = key_create(9);
	tab[N++] = key_create(16);
	tab[N++] = key_create(1);
	tab[N++] = key_create(17);

	for (i = 0; i < N; i++)
	{
		rbtree_insert(tree, tab[i]);
		rbtree_to_dot(tree, racine, dossier);
		if (mode & VERBOSE)
			rbtree_map_debug(tree);
	}

	for (i = i - 1; i >= 0; i--)
	{
		if (mode & VERBOSE)
			printf("On supprime %d \n", key_put(tab[i]));
		rbtree_remove(tree, tab[i]);
		if (!rbtree_empty(tree))
		{
			rbtree_to_dot(tree, racine, dossier);
			if (mode & VERBOSE)
				rbtree_map_debug(tree);
		}
	}

	return 0;
}
