#include "stack.h"

int main()
{
	int i = 1,value;
	stack_t *s = NULL;
	s = stack_init(10);
	while(i <= 10) {
		push(s,i++);
	}
	while(s->clen != 0) {
		pop(s,&value);
		printf("%d ",value);
	}
	printf("\n");
	return 0;
}
