#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <string.h>
#include <unistd.h>

unsigned char pu[]={
  0x13, 0x11, 0x12, 0x06, 0x13, 0x12, 0x11, 0x12, 0x06 
};
unsigned char time[] ={
	2, 2, 2, 2, 0.5, 0.5, 0.5, 0.5, 2
} ;

unsigned int yindiao [3][7] = {
	261, 293, 329, 349, 391, 440, 494,
	523, 587, 659, 698, 783, 880, 988, 
	1046, 1174, 1318, 1396, 1567, 1975
} ;
#define CMD_BEEP_ON  _IO ('k', 0)
#define CMD_BEEP_OFF _IO ('k', 1)
#define CMD_BEEP_FRE _IO ('k', 2)
int main(void) 
{
	int i = 0 ;
	int fre , timer;		
	int fd = open ("/dev/beep", O_RDWR); 
	if (fd < 0) 
		perror ("FD ERROR!") ;

	ioctl (fd, CMD_BEEP_ON, 0) ;
	
	while (1) {
		for (i = 0; i < sizeof (pu) ; i++)
		{	
			fre = pu[i] ;
			fre = yindiao[fre>> 4][(fre & 0xf)-1] ;
			ioctl (fd, CMD_BEEP_FRE, fre) ;
			timer = time[i] ;
			usleep (timer * 250000) ;
		}
	}
	ioctl (fd, CMD_BEEP_OFF, 0) ;
	return 0 ;
}

