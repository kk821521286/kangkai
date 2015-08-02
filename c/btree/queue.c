#include "queue.h"

node_t * create_node(qdata_t value)
{
	node_t *node = NULL;
	node = malloc(sizeof(node_t));
	node->value = value;
	node->next = NULL;
	return node;
}
queue_t * queue_init(int length)
{
	queue_t *q = NULL;
	q = malloc(sizeof(queue_t));
	q->tlen = length;
	q->clen = 0;
	q->head = create_node(0);
	q->tail = q->head;
	return q;
}

int enqueue(queue_t * q,qdata_t value)
{
	if(q->clen == q->tlen)
		return -1;
	q->tail->next = create_node(value);
	q->tail = q->tail->next;
	q->clen++;
	return 0;
}

int dequeue(queue_t *q,qdata_t *value)
{
	node_t *temp = NULL;
	if(q->clen == 0) 
		return -1;
	
	temp = q->head;
	*value = temp->next->value;
	q->head = temp->next;
	free(temp);
	q->clen--;
	return 0;
}
int queue_destroy(queue_t *q)
{
	node_t *temp = NULL,*p = NULL;
	if(!q->head)
		return -1;
	temp = q->head;
	while(temp) {
		p = temp;
		temp = temp->next;
		free(p);
	}
	free(q);
	return 0;
}
