#include <stdio.h>

void FindCredit(int **);

int main()
{
	int vals[] = {7,6,5,4,3,2,-1,0};
	int *fp = vals;
	FindCredit(&fp);
	printf("数组中的负数为：%d\n",*fp);
	return 0;
}

void FindCredit(int **ff)
{
	while(*ff != NULL)
		if(**ff < 0) break;
		else 
		(*ff)++;
}
