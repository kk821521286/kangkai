#include <stdio.h>

int main()
{
	char *ptr = "abcd";
	
	printf("sizeof(*ptr) = %d\n",sizeof(*ptr));
	printf("sizeof(1) = %d\n",sizeof(1));
	printf("sizeof((double)1) = %d\n",sizeof((double)1));
	printf("sizeof('1') = %d\n",sizeof('1'));
	printf("sizeof(ptr) = %d\n",sizeof(ptr));
	printf("sizeof(\"abcd\") = %d\n",sizeof("abcd"));
	printf("sizeof(ptr + 3) = %d\n",sizeof(ptr + 3));
	printf("sizeof(ptr + 1) = %d\n",sizeof(ptr + 1));
	printf("sizeof(*ptr + 1) = %d\n",sizeof(*ptr + 1));
	printf("sizeof(*ptr + '1') = %d\n",sizeof(*ptr + '1'));
	return 0; 
}
