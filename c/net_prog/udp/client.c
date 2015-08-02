#include "head.h"

#define MAX_BUF_SIZE 1024


int main(int argc,char **argv) 
{
	int sockfd,port;
	struct sockaddr_in addr;
	if (argc != 3) {
		fprintf(stderr,"Usage:%s server_ip server_port\n",argv[0]);
		exit(1);
	}

	if((port = atoi(argv[2])) < 0) {
		fprintf(stderr,"Usage:%s server_ip server_port\n",argv[0]);
		exit(1);
	}

	sockfd = socket(AF_INET,SOCK_DGRAM,0);
	if (sockfd < 0) {
		fprintf(stderr, "Socket Error:%s\n",strerror(errno));
		exit(1);
	}

	/*填充服务端的资料*/
	bzero(&addr,sizeof(struct sockaddr_in));
	addr.sin_family = PF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(argv[1]);
	/*
	if( bind(sockfd,(struct sockaddr*)&addr,sizeof(struct sockaddr_in)) < 0) {
		fprintf(stderr,"Bind Error:%s\n",strerror(errno));
		exit(1);
	}
	*/
	char buffer[MAX_BUF_SIZE];
	int n,ret ;
	printf(">>");
	while(fgets(buffer,MAX_BUF_SIZE,stdin)) {
		/*从键盘上读入，写到服务器*/
		ret = sendto(sockfd,buffer,strlen(buffer),0,(struct sockaddr*)&addr,sizeof(addr));
		printf("ret = %d\n",ret);
		if (ret == -1) {
			fprintf(stderr,"sendto error:%s\n",strerror(errno));
			exit(1);
		}
		ret = strlen(buffer);
		puts("send success");
		//printf("buffer = %s\n",buffer);
	//	bzero(buffer,MAX_BUF_SIZE);
		/*从网络上读，写到屏幕上*/
		memset(buffer,0,sizeof(buffer));
		n = recvfrom(sockfd,buffer,ret,0,NULL,NULL);
		if (n <= 0) {
			fprintf(stderr,"Recv Error:%s\n",strerror(errno));
			exit(1);
		}
	//	printf("n = %d\n",n);
		buffer[n] = '\0';
		fprintf(stdout,"get %s",buffer);
		printf(">>");
	}
	close(sockfd);
	exit(0);
}


