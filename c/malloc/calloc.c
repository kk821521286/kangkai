#include <stdio.h>
#include <stdlib.h>

int main()
{
	char ch[5] = {'a','b','c','d','e'};
	int i = 0;
	double temp[5] = {1001,2002,3003,4004,5005};
	double *p = calloc(5,8);

	
	for(i = 0; i < 5; i++) {
		*(p + i) = temp[i];
	}

	for(i = 0; i < 5; i++) {
		printf("%lf ",*(p + i));
	}
	printf("%d\n",sizeof(double));
	return 0;
}
