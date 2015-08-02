#include "queue.h"

int main()
{
	int i  = 1,ret ,value;
	queue_t *q = NULL;
	q = queue_init(100);
	while(i < 20) {
		if(ret == -1) 
			break;
		ret = enqueue(q,i++);
	}
	
	while(q->clen != 0) {
		dequeue(q,&value);
		printf("%d ",value);
	}
	printf("\n");
	destroy(q);
	
	return 0;
}
