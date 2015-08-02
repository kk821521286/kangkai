#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
	int fd = open ("/dev/poll", O_RDWR);
	char buf [128] ;
	if (fd < 0) {
		perror ("open error!\n") ;
	} 
	
	while (1) {
		if (getchar () == 'w') 
			write (fd, buf, 1) ;
	}
	return 0; 
}
