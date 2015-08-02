#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>

#define CMD_SET_BAUD _IO ('k', 0)
#define CMD_SET_DATA _IO ('k', 1)
#define CMD_SET_ODD  _IO ('k', 2)    

int main(void) 
{
	int ret = 0 ;
	char buf [128] = "aaaaaaaaaaaa";
	// mknod /dev/hello_driver c 249 0
	int fd = open ("/dev/hello_driver", O_RDWR) ;
	if (fd < 0) {
		perror ("open err");
	}

	ioctl (fd, CMD_SET_BAUD, 9600) ;
	ioctl (fd, CMD_SET_DATA, 8) ;
	ioctl (fd, CMD_SET_ODD, 1111) ;
	write (fd, buf, sizeof(buf)) ;
	ret = read (fd, buf, sizeof(buf)) ;
	if (ret < 0) 
		perror ("read err!") ;

	printf ("read = %s\n", buf) ;
	close (fd) ;
	return 0 ;
}

