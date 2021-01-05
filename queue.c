/*
	queue.c
	------

	Par Mael Valais (mael.valais@univ-tlse3.fr) le 14-02-2013

 */

#include <stdlib.h> /* Pour malloc */
#include <assert.h>
#include "queue.h"

struct cell
{
	struct item *item;
	struct cell *next;
};

struct queue
{
	struct cell *head, *tail;
	int size;
};

void queue_new(struct queue **q)
{
	// On créé un pointeur
	*q = (struct queue *)malloc(sizeof(struct queue));
	(*q)->head = NULL;
	(*q)->tail = NULL;
	(*q)->size = 0;
}

int queue_empty(struct queue *q) { return q->head == NULL; }
int queue_full(struct queue *q) { return malloc(sizeof(struct cell)) == NULL; }

void queue_add(struct queue *q, struct item *v)
{
	assert(!queue_full(q));
	struct cell *new = (struct cell *)malloc(sizeof(struct cell));
	new->item = v;
	new->next = NULL;
	if (queue_empty(q))
	{				   /* L'ajout se fait a la fin de la file */
		q->tail = new; /* Pas besoin de dire que head->next = NULL */
		q->head = new;
	}
	else
	{
		q->tail->next = new;
		q->tail = new;
	}
	++(q->size);
}

struct item *queue_read(struct queue *q)
{
	assert(!queue_empty(q));
	return q->head->item;
}

struct item *queue_remove(struct queue *q)
{
	assert(!queue_empty(q));
	struct cell *temp = q->head;
	struct item *item = temp->item;
	q->head = q->head->next;
	free(temp);
	--(q->size);
	return (item);
}

int queue_size(struct queue *q)
{
	return q->size;
}
