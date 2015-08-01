#include <stdio.h>
#include <stdlib.h>
int main(int argc,char *argv[])
{
	char *p,*q;
	p = (char*)malloc(10);
	q = p;
	p = (char*)realloc(p,1000);
	
	printf("p=0x%x\n",p);
	printf("q=0x%x\n",q);
	return 0;
}
