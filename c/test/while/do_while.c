#include <stdio.h>

int main()
{
	int count = 0;
	do {
	/*
		if(count < 5)
		continue;
	*/	
		//count++;
	}while(count++ < 5);//为假则退出循环

	printf("%d\n",count);

	return 0;
}
