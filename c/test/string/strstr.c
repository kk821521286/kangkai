#include <stdio.h>
#include <string.h>

int main()
{
	char *c = NULL;
	char c1[] = "kangkai";
	char c2[] = "kai";
	c = strstr(c1,c2);

	if(c != NULL)
	printf("the string you want to find is %s\n",c);
	else
	printf("no found the string \n");
	return 0;
}
