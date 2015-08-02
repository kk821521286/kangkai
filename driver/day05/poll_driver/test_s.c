#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <errno.h>

int main(void)
{
	fd_set rfd ;
	struct timeval timeout ;
	int poll_fd = open ("/dev/poll", O_RDWR) ;
	if (poll_fd < 0) {
		perror ("open error!\n") ;
		return 0;
	}

	timeout.tv_sec = 0 ;
	timeout.tv_usec = 0 ;

	while (1) { 
		int ret = 0 ;
		FD_ZERO (&rfd) ;
		//FD_SET (0, &rfd) ;
		FD_SET (poll_fd, &rfd) ;

		//if ((ret = select (poll_fd + 1, &rfd, NULL, NULL, &timeout)) < 0) {
		
		if ((ret = select (poll_fd + 1, &rfd, NULL, NULL, NULL)) < 0) {
			perror ("Select Error !") ;
			return -1 ;
		}
		
		if (0 == ret) { 
			//printf ("Select Timeout\n") ;
		}

		if (ret > 0) {
			if (FD_ISSET (poll_fd, &rfd)) {
				printf ("poll call\n") ;
			}
		}
	}

	return 0;
}


