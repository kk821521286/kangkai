#include <stdio.h>
#include <string.h>

char * mystrstr(char * s1,char * s2);
int main()
{
	char *c = NULL;
	char c1[] = "kangkai";
	char c2[] = "kai";
	c = mystrstr(c1,c2);

	if(c != NULL)
	printf("the string you want to find is %s\n",c);
	else
	printf("no found the string \n");
	return 0;
}
char * mystrstr(char * s1,char * s2)
{
	char *pChar;
	pChar = s2;
	int len = strlen(s2);

	for(;strchr(s1,*pChar)!='\0';s1++)
	{
		if(strncmp(s1,pChar,len) == 0)
		{
			return s1;
		}
	}

	return   NULL;
}
