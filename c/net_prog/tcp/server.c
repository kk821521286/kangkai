#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>

int main(int argc,char **argv) {
	int sockfd,new_fd;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	int sin_size,portnumber;
	char hello[]="hello! welcome to server";

	if (argc != 2) {
		fprintf(stderr,"Usage:%s portnumber\n",argv[0]);
		exit(1);
	}

	if ((portnumber = atoi(argv[1])) < 0) {
		fprintf(stderr,"Usage:%s portnumber\n",argv[0]);
		exit(1);
	}

	if ((sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1) {
		fprintf(stderr,"Socket error:%s\n",strerror(errno));
		exit(1);
	}

	bzero(&server_addr,sizeof(struct sockaddr_in));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(portnumber);

	/*bind sockfd 描述符*/
	if (bind(sockfd,(struct sockaddr*)(&server_addr),sizeof(struct sockaddr)) == -1) {
		fprintf(stderr,"Bind error:%s\n",strerror(errno));
		exit(1);
	}

	/*监听sockfd描述符*/
	if (listen(sockfd,5) == -1 ) {
		fprintf(stderr,"Listen error:%s\n",strerror(errno));
		exit(1);
	}

		printf("start connection\n");
		/*服务器阻塞，直到客户程序建立连接*/
		sin_size = sizeof(struct sockaddr_in);
		if ((new_fd = accept(sockfd,(struct sockaddr*)(&client_addr),&sin_size)) == -1) {
			fprintf(stderr,"Accept error:%s\n",strerror(errno));
			exit(1);
		}
		fprintf(stderr,"server get connection from %s\n",inet_ntoa(client_addr.sin_addr));
		/*
		if(write(new_fd,hello,strlen(hello)) == -1) {
			fprintf(stderr,"write error:%s\n",strerror(errno));
			exit(1);
		}
		*/
	while(1) {
		bzero(hello,sizeof(hello));
		printf(">>");
		fgets(hello,sizeof(hello),stdin);
		if(send(new_fd,hello,strlen(hello),0) == -1 ) {
			fprintf(stderr,"send error:%s\n",strerror(errno));
			exit(1);	
		}
		if(strncmp(hello,"exit",4) == 0) {
			break;
		}
	}
	close(sockfd);
	exit(0);
}
