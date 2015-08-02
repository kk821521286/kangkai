#include <errno.h>  
#include <string.h>
#include <math.h>  
#include <stdio.h>  
int main(void)  
{  
	errno = 0;  
	if (NULL == fopen("1.txt", "rb"))  
	{  
		printf("%d\n", errno);  
		printf("%s\n",strerror(errno));
	}  
	else  
	{  
		printf("%d\n", errno);
		printf("open successful\n");
	}  
	return 0;  
} 
