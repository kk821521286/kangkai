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

int main(int argc,char *argv[]) {
	int sockfd;
	char buffer[1024];
	struct sockaddr_in server_addr;
	struct hostent *host;
	int portnumber,nbytes;

	if (argc != 3) {
		fprintf(stderr,"Usage:%s hostname portnumber\n",argv[0]);
		exit(1);
	}

	if((host = gethostbyname(argv[1])) == NULL) {
		fprintf(stderr,"Gethostname error\n");
		exit(1);
	}

	if((portnumber = atoi(argv[2])) < 0) {
		fprintf(stderr,"Usage:%s hostname portnumber\n",argv[0]);
		exit(1);
	}

	/*客户程序开始建立，sockfd描述符*/ 
	if((sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1 ) {
		fprintf(stderr,"Socket Error:%s\n",strerror(errno));
		exit(1);
	}

	bzero(&server_addr,sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(portnumber);
	server_addr.sin_addr = *((struct in_addr*)host->h_addr);
	
	printf("start connection\n");
	/*客户程序发起连接请求*/
	if (connect(sockfd,(struct sockaddr*)(&server_addr),sizeof(struct sockaddr)) == -1) {
		fprintf(stderr,"Connect Error:%s\n",strerror(errno));
		exit(1);
	}
	while(1) {
	bzero(buffer,sizeof(buffer));
	
	sleep(1);
	/*连接成功了*/
	if ((nbytes = recv(sockfd,buffer,1024,0)) == -1) {
		fprintf(stderr,"Read Error:%s\n",strerror(errno));
		exit(1);
	}
	printf("nbytes = %d\n",nbytes);
	buffer[strlen(buffer)] = '\0';
	printf("i have received:%s\n",buffer);
	if(strncmp(buffer,"exit",4) == 0) {
		break;
	}
	}
	if(-1 == close(sockfd)) {
		fprintf(stderr,"close Error:%s\n",strerror(errno));
		exit(1);
	}
	/*结束通讯*/
	exit(0);
}
