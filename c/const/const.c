#include <stdio.h>

int main()
{
	int t = 10;
	int a = 8; //const int a is same as int const a
	int * const b = &a; //b is a constant pointer only point to &a
	*b = 88;
	printf("a = %d\n",a);


	const int * c = &t; //c is a constant 
	
	const  int * const d = &t;

	printf("d = %d\n",*d);

	return 0;
}
