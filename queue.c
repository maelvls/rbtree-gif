/*
 * queue.c
 *
 * A naive FIFO queue implementation. Not thread-safe. Each push operation
 * allocates a new item to the heap. Not great, but works fine for the
 * purposes of this tiny project.
 *
 *
 *   tail                head
 *  +---+---+---+---+---+---+
 *  | 1 | 4 | 2 | 0 | 2 | 5 |
 *  +---+---+---+---+---+---+
 *  <---------------------->
 *            size
 *
 * In this example, queue_push adds the element to the tail of the queue
 * (leftmost element of the above diagram) and queue_pop deletes the
 * elements at the head of the queue (rightmost element in the diagram).
 *
 * Copyright (C) 2013-2020  Mael Valais
 */

#include "queue.h"

#include <stdlib.h> /* malloc */
#include <assert.h>
#include <errno.h>

struct cell
{
	item *item;
	struct cell *next;
};

struct queue
{
	struct cell *head, *tail;
	int size;
};

/*
 * queue_new returns an empty queue.
 */
struct queue *queue_new()
{
	struct queue *q = (struct queue *)malloc(sizeof(struct queue));
	q->head = NULL;
	q->tail = NULL;
	q->size = 0;

	return q;
}

int queue_is_empty(struct queue *q) { return q->head == NULL; }

/*
 * queue_push adds the given element to the tail of the queue. Returns 0 on
 * success, or an error code otherwise.
 */
int queue_push(struct queue *q, void *new_head)
{
	struct cell *new = (struct cell *)malloc(sizeof(struct cell));
	if (new < 0)
	{
		return -ENOMEM;
	}

	new->item = new_head;
	new->next = NULL;
	if (queue_is_empty(q))
	{
		q->tail = new;
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

/*
 * queue_head returns the head of the queue. This function must not be
 * called when the queue is empty.
 */
void *queue_head(struct queue *q)
{
	assert(!queue_is_empty(q));
	return q->head->item;
}

/*
 * queue_pop deletes and returns the head element of the queue. This
 * function must not be called when the queue is empty.
 */
void *queue_pop(struct queue *q)
{
	assert(!queue_is_empty(q));
	struct cell *temp = q->head;
	void *item = temp->item;
	q->head = q->head->next;
	free(temp);
	--(q->size);
	return (item);
}

int queue_size(struct queue *q)
{
	return q->size;
}
