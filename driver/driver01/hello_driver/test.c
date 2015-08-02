#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>

int main(void) 
{
	// mknod /dev/hello_driver c 249 0
	int fd = open ("/dev/hello_driver", O_RDWR) ;
	printf("fd=%d\n",fd);
	if (fd == -1) 
		printf("drvier call fail\n");
	else 
		printf("driver call sucess\n");
	close (fd) ;
	return 0 ;
}

