#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdlib.h>

typedef struct __node {
	int value;
	struct __node *next;
}node_t;
typedef struct __queue {
	node_t *head;
	node_t *tail;
	int clen;
	int tlen;
}queue_t;

extern node_t * create_node(int value);
extern int enqueue(queue_t *q,int value);
extern int dequeue(queue_t *q,int *value);
extern queue_t *queue_init(int length);
extern int destroy(queue_t *q);

#endif
