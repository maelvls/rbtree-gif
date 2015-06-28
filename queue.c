/*
	queue.c
	------
	
	Par Mael Valais (mael.valais@univ-tlse3.fr) le 14-02-2013	

 */

#include <stdlib.h> /* Pour malloc */
#include <assert.h>
#include "queue.h"

typedef struct _cell {	
	ITEM item;
	struct _cell* next;
} *CELL;

struct _queue {	
	CELL head;
	CELL tail;
	int size;
};

void queueCreate(QUEUE *q) { 	
	// On créé un pointeur
	*q = (QUEUE)malloc(sizeof(struct _queue));
	(*q)->head = NULL;
	(*q)->tail = NULL;
	(*q)->size = 0;
}

int queueEmpty(QUEUE q) { return q->head == NULL; }
int queueFull(QUEUE q) { return malloc(sizeof(CELL)) == NULL; }

void queueAdd(QUEUE q, ITEM v) {	
	assert(!queueFull(q));
	CELL new = (CELL)malloc(sizeof(struct _cell));
	new->item = v;
	new->next = NULL;
	if (queueEmpty(q)) {	/* L'ajout se fait a la fin de la file */
		q->tail = new; 		/* Pas besoin de dire que head->next = NULL */
		q->head = new;
	}
	else {
		q->tail->next = new;
		q->tail = new;
	}
	++(q->size);
}

ITEM queueRead(QUEUE q) {
	assert(!queueEmpty(q));
	return q->head->item;
}

ITEM queueRemove(QUEUE q) {	
	assert(!queueEmpty(q));
	CELL temp = q->head;
	ITEM item = temp->item;
	q->head = q->head->next;
	free(temp);
	--(q->size);
	return(item);
}

int queueSize(QUEUE q) {	
	return q->size;
}
