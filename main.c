/*
 * main.c
 *
 * This is the rbtree_to_dot program. This program does operations to a
 * simple red-black tree in step-by-step mode. These operations (additions
 * and removals) are hardcoded below. The program outputs each step of the
 * tree as dot files (e.g., it shows the rebalancing algorithm) that can be
 * then converted to a nice GIF that shows how a red-black tree evolves.
 *
 * Copyright (C) 2013-2020  Mael Valais
 *  */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <dirent.h>

#include "rbtree.h"
#include "key.h"

#define DOTFILES_DIR "dot"
#define DOTFILES_PREFIX "step_"

#define VERBOSE 1

int main(int argc, const char *argv[])
{
	char dotfiles_dir[30] = DOTFILES_DIR;
	char dotfiles_prefix[30] = DOTFILES_PREFIX;
	int i, mode = 0;
	DIR *d;

	// Parse command-line arguments.
	for (i = 1; i < argc; ++i)
	{
		if (argv[i][0] != '-')
		{
			strcpy(dotfiles_dir, argv[i]); /* c'est le nom du dossier */
		}
		if (0 == strcmp(argv[i], "-h") || 0 == strcmp(argv[i], "--help"))
		{
			printf("usage: %s [-h] [output_dotfiles_dir] [-r output_dotfiles_prefix] \n", argv[0]);
			printf("\n");
			printf("By default, the output dir is '%s'.\n", DOTFILES_DIR);
			printf("Run -h or --help to see more.\n");
			exit(0);
		}
		if (strcmp(argv[i], "-r") == 0)
		{
			if (i + 1 < argc)
			{
				strcpy(dotfiles_prefix, argv[i + 1]);
			}
			else
			{
				fprintf(stderr, "error: the -r flag takes an argument\n");
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
	if ((d = opendir(dotfiles_dir)))
	{
		closedir(d);
	}
	else
	{
		fprintf(stderr, "error: the directory '%s' does not exist, please create it first\n", dotfiles_dir);
		exit(1);
	}

	struct rbtree *tree = rbtree_new(key_cmp, key_equal);
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
		rbtree_to_dot(tree, dotfiles_prefix, dotfiles_dir);
		if (mode & VERBOSE)
			rbtree_map_debug(tree);
	}

	for (i = i - 1; i >= 0; i--)
	{
		if (mode & VERBOSE)
			printf("removing %d \n", key_put(tab[i]));
		rbtree_remove(tree, tab[i]);
		if (!rbtree_empty(tree))
		{
			rbtree_to_dot(tree, dotfiles_prefix, dotfiles_dir);
			if (mode & VERBOSE)
				rbtree_map_debug(tree);
		}
	}

	return 0;
}
