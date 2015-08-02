#ifndef QUEUE_H
#define QUEUE_H
#include <stdio.h>
#include <stdlib.h>
#include "btree.h"

typedef BNode *qdata_t;
typedef struct __node {
	qdata_t value;
	struct __node *next;
}node_t;
typedef struct __queue {
	node_t *head;
	node_t *tail;
	int clen;
	int tlen;
}queue_t;

extern node_t * create_node(qdata_t value);
extern int enqueue(queue_t *q,qdata_t value);
extern int dequeue(queue_t *q,qdata_t *value);
extern queue_t *queue_init(int length);
extern int queue_destroy(queue_t *q);

#endif
