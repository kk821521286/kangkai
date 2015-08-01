#include <stdio.h>

int main()
{
	int a1 ,a2,a3,a4 ,a5;
	a1 = 0x87;
	a2 = 0x65; 
	a3 = 0x43;
	a4 = 0x21;
	a5 = a1 >> 3 | a2 >> 2 | a3 >> 1 | a4;

	printf("0x%x\n",a5);

	return 0;
}
