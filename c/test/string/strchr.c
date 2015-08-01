#include <stdio.h>
#include <string.h>

int main()
{
	char c  = 'i';
	char c1[] = "kangkai";
	char *c2 = NULL;
	c2 = strchr(c1,c);

	if(c2 != NULL)
	printf("the charactor you want to find is %c\n",*c2);
	else
	printf("no found the charactor \n");
	return 0;
}
