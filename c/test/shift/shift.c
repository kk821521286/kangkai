#include <stdio.h>

int main()
{
	unsigned int a1 = 0xAA;
	unsigned int a2 = 0xBB;
	unsigned int a3 = 0xCC;
	unsigned int a4 = 0xDD;

	unsigned int a5 ;
	a5 = a1 << 24 | a2 << 16 | a3 << 8 | a4  ;
	printf("0x%x\n",a5);
	return 0;
}
