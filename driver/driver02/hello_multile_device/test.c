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
	int fd [2] ;

	fd [0] = open ("/dev/hello0", O_RDWR) ;
	fd [1] = open ("/dev/hello1", O_RDWR) ;
	return 0 ;
}

