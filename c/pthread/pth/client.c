#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<string.h>
#include<sys/types.h>
#include<sys/time.h>

int main(){
	int result;
	int socketfd;
	int len;
	struct sockaddr_un address;
	fd_set read_fds, test_fds;
	int fd;    
	int max_fds;
	char buffer[1024];

	socketfd = socket(AF_UNIX, SOCK_STREAM, 0);

	address.sun_family = AF_UNIX;
	strcpy(address.sun_path, "server_socket");
	len = sizeof(address);

	result = connect(socketfd, (struct sockaddr*)&address, len);
	if(result == -1){
		perror("connect");
		exit(EXIT_FAILURE);
	}

	FD_ZERO(&read_fds);
	FD_SET(0, &read_fds);
	FD_SET(socketfd, &read_fds);
	max_fds = socketfd +1;

	printf("Chat now!!\n");

	while(1){
		test_fds = read_fds;
		result = select(max_fds, &test_fds, (fd_set *)NULL, (fd_set *)NULL, (struct timeval*)NULL);
		if(result < 1){
			perror("select");
			exit(EXIT_FAILURE);
		}

		if(FD_ISSET(0, &test_fds)){
			memset(buffer, '\0', sizeof(buffer));
			//    printf("send:");
			fgets(buffer, sizeof(buffer), stdin);
			if((strncmp("quit", buffer, 4))== 0){
				printf("\nYou are going to quit\n");
				break;
			}
			result = write(socketfd, buffer, sizeof(buffer));
			if(result == -1){
				perror("write");
				exit(EXIT_FAILURE);
			}    
		}
		if(FD_ISSET(socketfd, &test_fds)){
			memset(buffer, '\0', sizeof(buffer));
			result = read(socketfd, buffer, sizeof(buffer));
			if(result == -1){
				perror("read");
				exit(EXIT_FAILURE);
			}else if(result == 0){
				printf("The other side has termianl chat!\n");
				break;
			}else{
				printf("recieve: %s", buffer);
			}
		}
	}
	close(socketfd);
	exit(EXIT_SUCCESS);
}
