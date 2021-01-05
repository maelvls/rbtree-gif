/*
 * A naive queue implementation.
 */

struct queue;

void queue_new(struct queue **q);
int queue_empty(struct queue *q);
int queue_add(struct queue *q, void *v);
void *queue_remove(struct queue *q);
void *queue_read(struct queue *q);
int queue_size(struct queue *q);
