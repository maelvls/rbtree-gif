/*
 * A naive queue implementation.
 */

typedef void item;
struct queue;

struct queue *queue_new();
int queue_is_empty(struct queue *q);
int queue_push(struct queue *q, item *v);
item *queue_pop(struct queue *q);
item *queue_head(struct queue *q);
int queue_size(struct queue *q);
