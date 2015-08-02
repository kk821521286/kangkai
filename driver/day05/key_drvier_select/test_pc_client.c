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

int main(int argc, char *argv[])
{
	struct sockaddr_in conn_addr; 
	char readline [1024] ;
	char recvbuf [1500] ;
	fd_set rfd ;
	
	if (2 != argc) {
		printf ("Usage Error: xxx serverip! \n") ;
		exit (0) ;
	}

	int conn_fd = socket (PF_INET, SOCK_STREAM, 0) ;
	if (conn_fd < 0) {
		perror ("Socket Create Error!\n") ;
		return -1 ;
	}
	
	bzero (&conn_addr, sizeof (conn_addr)) ;
	conn_addr.sin_family = AF_INET ;
	conn_addr.sin_port = htons (8888) ;
	conn_addr.sin_addr.s_addr = inet_addr (argv[1]) ;
	if (connect (conn_fd, (struct sockaddr*)&conn_addr, sizeof (conn_addr)) < 0) {
		perror ("Connect to server error") ;
		exit (0) ;
	} 

	printf ("Connect To Server !\n") ;
	while (1) { 
		int ret = 0 ;
		FD_ZERO (&rfd) ;
		FD_SET (0, &rfd) ;
		FD_SET (conn_fd, &rfd) ;

		if ((ret = select (conn_fd + 1, &rfd, NULL, NULL, NULL)) < 0) {
			perror ("Select Error !") ;
			close (conn_fd) ;
			return -1 ;
		}
		
		if (0 == ret) 
			printf ("Select Timeout\n") ;

		if (ret > 0) {
			if (FD_ISSET (conn_fd, &rfd)) {
				bzero (recvbuf, sizeof (recvbuf));
				if (recv (conn_fd, recvbuf, sizeof (recvbuf), 0) <= 0) {
					printf ("Recv Error !\n");
					close (conn_fd) ;
					return -1 ;
				}
				printf ("client say : %s \n", recvbuf) ;
			}

			if (FD_ISSET(0, &rfd)) {
				bzero (readline, sizeof (readline)) ;
				fgets (readline, sizeof (readline), stdin) ;
				send (conn_fd, readline, sizeof (readline), 0) ;
			}
		}
	}

	return 0;
}


