#include <stdio.h>

int main(void)
{
	int a[5] = {1,2,3,4,5};
	int b[6] = {3,4,5,7,8,9};
	int *pa = a;
	int *pb = b;
	int i = 0,count = 0;
	
	for(i = 0; i < sizeof(a) / sizeof(a[0]) ; i++) {
		if(pb == &b[6]) 
			break;
		if(*pa == *pb) {
			++count;
			++pa;
			++pb;
		} else if (*pa < *pb) {
			++pa;
		} else {
			++pb;
		}
	}
	printf("two arrays same count is %d \n",count);
	return 0;
}
