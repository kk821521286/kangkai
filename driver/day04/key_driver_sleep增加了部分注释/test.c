#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
	int fd = open ("/dev/key", O_RDWR);
	char key ;
	int flags ;

	if (fd < 0) {
		perror ("open error!\n") ;
	} 
#if 0	
	flags = fcntl (fd, F_GETFL, 0) ;
	flags |= O_NONBLOCK ;
	fcntl (fd, F_SETFL, flags) ;
#endif
	while (1) {
		memset (&key, 0, sizeof (key));
		read (fd, &key, sizeof (key)) ;
		printf ("key = %d\r\n", key) ;
	}
	return 0; 
}
