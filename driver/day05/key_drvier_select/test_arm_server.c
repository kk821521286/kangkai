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
	struct sockaddr_in serveraddr; 
	char readline [1024] ;
	char recvbuf [1500] ;
	fd_set rfd ;
	char *p_send = "key down !\n" ;
	int client_fd, listen_fd, key_fd ;

	//mknod /dev/key c 249 0 
	key_fd = open ("/dev/key", O_RDWR) ;
	if (key_fd < 0) {
		perror ("Open key device Error!") ;
		exit (0) ;
	}
	
	listen_fd = socket (PF_INET, SOCK_STREAM, 0) ;
	if (listen_fd < 0) {
		perror ("Socket Create Error!\n") ;
		return -1 ;
	}
	
	bzero (&serveraddr, sizeof (serveraddr)) ;
	serveraddr.sin_family = AF_INET ;
	serveraddr.sin_port = htons (8888) ;
	serveraddr.sin_addr.s_addr = htonl (INADDR_ANY) ;
	if (bind (listen_fd, (struct sockaddr*)&serveraddr, sizeof (serveraddr)) < 0) {
		perror ("Bind Error!") ;
		return -1; 
	}

	listen (listen_fd, 5) ;
	client_fd = accept (listen_fd, NULL, NULL);
	if (client_fd < 0) {
		perror ("Accept Error!") ;
		return -1 ;
	}

	printf ("New Client Accept !\n") ;
	while (1) { 
		int ret = 0 ;
		int max_fd ; 
		FD_ZERO (&rfd) ;
		FD_SET (key_fd, &rfd) ;
		FD_SET (client_fd, &rfd) ;

		max_fd = (key_fd > client_fd) ? key_fd : client_fd ;
		if ((ret = select (max_fd + 1, &rfd, NULL, NULL, NULL)) < 0) {
			perror ("Select Error !") ;
			close (client_fd) ;
			return -1 ;
		}
		
		if (0 == ret) 
			printf ("Select Timeout\n") ;

		if (ret > 0) {
			if (FD_ISSET (client_fd, &rfd)) {
				bzero (recvbuf, sizeof (recvbuf));
				if (recv (client_fd, recvbuf, sizeof (recvbuf), 0) <= 0) {
					printf ("Recv Error !\n");
					close (client_fd) ;
					return -1 ;
				}
				printf ("client say : %s \n", recvbuf) ;
			}

			if (FD_ISSET(key_fd, &rfd)) {
				bzero (readline, sizeof (readline)) ;
				//fgets (readline, sizeof (readline), stdin) ;
				read (key_fd, readline, sizeof(readline)) ;
				send (client_fd, p_send, strlen (p_send), 0) ;
			}
		}
	}

	return 0;
}


