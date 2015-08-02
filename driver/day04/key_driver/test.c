#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

int main(void)
{
	int fd = open ("/dev/key", O_RDWR);
	char buf [128] ;
	if (fd < 0) {
		perror ("open error!\n") ;
	} 

//	read (fd, buf, sizeof (buf)) ;
	
	while (1) ;
	return 0; 
}
