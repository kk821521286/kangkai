#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
	int fd = open ("/dev/sleep_task", O_RDWR | O_NONBLOCK);
	char key ;
	if (fd < 0) {
		perror ("open error!\n") ;
		return -1 ;	
	} 
#if 0	
	flags = fcntl (fd, F_GETFL, 0) ;
	flags |= O_NONBLOCK ;
	fcntl (fd, F_SETFL, flags) ;
#endif
	while (1) {
		memset (&key, 0, sizeof (key));
		read (fd, &key, sizeof (key)) ;
		printf ("key read\r\n", key) ;
	}
	return 0; 
}
