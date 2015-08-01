#include "stack.h"


void init(stack *s)
{
	int i;
	for(i=0;i<MAX;i++)
		s->a[i] = 0;
	s->top = -1;
}
int pop(stack *s,int *value)
{
	if(s->top == -1) {
		return -1;
	}
	*value = s->a[s->top--] ;
	return 0;
}

int push(stack *s,int value)
{	
	if(s->top > MAX - 1)
		return -1;
		s->top++;
	s->a[s->top] = value;
	return 0;
}
