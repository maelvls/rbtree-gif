/*
	queue.h
	------

	Par Mael Valais (mael.valais@univ-tlse3.fr) le 14-02-2013

 */

#ifndef __queue_h__
#define __queue_h__
struct queue;

void queue_new(struct queue **q);
int queue_empty(struct queue *q);
int queue_add(struct queue *q, void *v);
void *queue_remove(struct queue *q);
void *queue_read(struct queue *q);
int queue_size(struct queue *q);

#endif /* !__queue_h__ */
