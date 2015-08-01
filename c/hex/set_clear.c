#include <stdio.h>

int main()
{
    double a = 72000000 * 1000;
	
	printf("a = 0x%08X\n",a);

	//a &=  (~(0x8 << 28));
	a &= ~(0x1 << 31);
	printf("a = 0x%08X\n",a);
	return 0;
}
