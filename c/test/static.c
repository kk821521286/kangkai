#include <stdio.h>

static int a = 0;
int main()
{
	int i = 0;
	for(i=0;i<100;i++)
		a++;
	printf("a=%d\n",a);
	a = 5;
	printf("a=%d\n",a);
	return 0;
}
