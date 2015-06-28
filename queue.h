/*
	queue.h
	------
	
	Par Mael Valais (mael.valais@univ-tlse3.fr) le 14-02-2013

 */

#ifndef __queue_h__
#define __queue_h__

typedef struct _queue* QUEUE;
typedef struct _node* ITEM;

void queueCreate(QUEUE *q);
int queueEmpty(QUEUE q);
int queueFull(QUEUE q);
void queueAdd(QUEUE q, ITEM v);
ITEM queueRead(QUEUE q);
ITEM queueRemove(QUEUE q);
int queueSize(QUEUE q);

#endif /* !__queue_h__ */

