#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdlib.h>

#define MAX 20
typedef struct queue_t {
	int a[MAX];
	int head;
	int tail;
}queue;

extern void init(queue *q);
extern int enqueue(queue *q,int value);
extern int dequeue(queue *q,int *value);
#endif  
