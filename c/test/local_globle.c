#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void test(char **p)
{
	*p = malloc(100);
}
int main()
{
	char *str = NULL;
	test(&str);
	strcpy(str,"hello world");
	printf("%s\n",str);
	return 0;
}
