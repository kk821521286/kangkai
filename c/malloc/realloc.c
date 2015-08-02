#include <stdio.h>
#include <stdlib.h>

int main()
{
	char ch[5] = {'a','b','c','d','e'};
	int i = 0;
	char *p = malloc(5);

	
	for(i = 0; i < 5; i++) {
		*(p + i) = ch[i];
	}
	
	for(i = 0;i < 5;i++) {
		printf("%c ",*(p + i));
	}
	printf("\n");
	p = realloc(p,sizeof(8));

	for(i = 0; i < 5; i++) {
		*(p + i) = ch[i];
	}
	for(i = 0;i < 5;i++) {
		printf("%c ",*(p + i));
	}
	printf("\n");

	return 0;
}
