/* ===========================================================
 *
 *       Filename:  key.c
 *
 *    Description:
 *
 *        Created:  06-04-2013 22:32
 *
 *         Author:  Mael Valais
 *         Mail  :	mael.valais@univ-tlse3.fr
 *
 * =========================================================== */

#include "key.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

void *keyCreer(int valeur)
{
	int *key = (int *)malloc(sizeof(int));
	*key = valeur;
	return key;
}

void keySuppr(void *key)
{
	assert(key != NULL);
	free(key);
}

int keyCmp(const void *key1, const void *key2)
{
	assert(key1 != NULL && key2 != NULL);
	int *a = (int *)key1;
	int *b = (int *)key2;
	return *a < *b;
}

int keyEqual(const void *key1, const void *key2)
{
	assert(key1 != NULL && key2 != NULL);
	int *a = (int *)key1;
	int *b = (int *)key2;
	return *a == *b;
}

int keyPut(void *key)
{ /* XXX DEBUG */
	assert(key != NULL);
	int *a = (int *)key;
	return a != NULL ? *a : -1;
}
