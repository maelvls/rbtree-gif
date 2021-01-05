/*
 * queue.c
 *
 * A naive queue implementation. Each push operation allocates a new item
 * to the heap. Not great, but works fine for the purposes of this tiny
 * project.
 *
 * Copyright (C) 2013-2020  Mael Valais
 */

#include "queue.h"

#include <stdlib.h> /* malloc */
#include <assert.h>
#include <errno.h>

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
	*q = (struct queue *)malloc(sizeof(struct queue));
	(*q)->head = NULL;
	(*q)->tail = NULL;
	(*q)->size = 0;
}

int queue_empty(struct queue *q) { return q->head == NULL; }

/*
 * Returns 0 on success, or an error code otherwise.
 */
int queue_add(struct queue *q, void *v)
{
	struct cell *new = (struct cell *)malloc(sizeof(struct cell));
	if (new < 0)
	{
		return -ENOMEM;
	}

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

	return 0;
}

void *queue_read(struct queue *q)
{
	assert(!queue_empty(q));
	return q->head->item;
}

void *queue_remove(struct queue *q)
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
