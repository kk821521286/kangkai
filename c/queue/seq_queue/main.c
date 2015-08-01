#include "queue.h"

int main()
{
	int i ;
	int value = 0,input, ret;
	queue  q ;
	init(&q);
	printf("请输入一个整数:");
	scanf("%d",&input);
	for(i=1;i<=input;i++) {
		ret = enqueue(&q,i);
		if (ret == -1) 
		break;
	}
	while(q.head != q.tail) {
		dequeue(&q,&value);
		printf("%d ",value);
	}
	return 0;
}
