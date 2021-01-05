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

	RBTree tree = rbtreeCreate(keyCmp, keyEqual);
	int N = 0;
	int *tab[100];

	tab[N++] = keyCreer(18);
	tab[N++] = keyCreer(0);
	tab[N++] = keyCreer(19);
	tab[N++] = keyCreer(23);
	tab[N++] = keyCreer(2);
	tab[N++] = keyCreer(24);
	tab[N++] = keyCreer(10);
	tab[N++] = keyCreer(3);
	tab[N++] = keyCreer(11);
	tab[N++] = keyCreer(8);
	tab[N++] = keyCreer(12);
	tab[N++] = keyCreer(13);
	tab[N++] = keyCreer(7);
	tab[N++] = keyCreer(14);
	tab[N++] = keyCreer(20);
	tab[N++] = keyCreer(4);
	tab[N++] = keyCreer(21);
	tab[N++] = keyCreer(5);
	tab[N++] = keyCreer(22);
	tab[N++] = keyCreer(6);
	tab[N++] = keyCreer(15);
	tab[N++] = keyCreer(9);
	tab[N++] = keyCreer(16);
	tab[N++] = keyCreer(1);
	tab[N++] = keyCreer(17);

	for (i = 0; i < N; i++)
	{
		rbtreeInsert(tree, tab[i]);
		rbtreeToDot(tree, racine, dossier);
		if (mode & VERBOSE)
			rbtreeMapDebug(tree);
	}

	for (i = i - 1; i >= 0; i--)
	{
		if (mode & VERBOSE)
			printf("On supprime %d \n", keyPut(tab[i]));
		rbtreeRemove(tree, tab[i]);
		if (!rbtreeEmpty(tree))
		{
			rbtreeToDot(tree, racine, dossier);
			if (mode & VERBOSE)
				rbtreeMapDebug(tree);
		}
	}

	return 0;
}
