#include "stack.h"

int main(int argc,char *argv[])
{
	stack s ;
	int i = 0,value,input = 0,temp = 0;
	init(&s);
	printf("请输入一个整数:");
	scanf("%d",&input);
	while(i < input) 
	push(&s,i++);
	
	while(s.top != -1) { 
	pop(&s,&value);
	if(temp % 5 == 0 )
	printf("\n");
	printf("%-5d ",value);
	temp++;
	}
	printf("\n");
	
	return 0;

}
