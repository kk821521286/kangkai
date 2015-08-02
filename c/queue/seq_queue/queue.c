#include "queue.h"

void init(queue *q) 
{
	int i = 0;
	for(i = 0; i < MAX;i++) 
	q->a[i] = 0;
	q->head = q->tail = 0;
	return ;
}

int enqueue(queue *q,int value)
{
	if((q->tail - q->head) == MAX - 1) 
		return -1;

	q->a[(q->tail++) % MAX] = value;
	q->tail = q->tail % MAX;
	return 0;
}

int dequeue(queue *q,int *value)
{
	if(q->tail == q->head) 
		return -1;
	*value = q->a[(q->head++)%MAX] ;
	q->head = q->head % MAX;
	return 0;
}
