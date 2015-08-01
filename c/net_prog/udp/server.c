#include "head.h"
#define SERVER_PORT 8888
#define MAX_MSG_SIZE 1024


int main(void)
{
	int sockfd,ret ;
	struct sockaddr_in addr;
	sockfd = socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd < 0) {
		fprintf(stderr,"Socket Error:%s\n",strerror(errno));
		exit(1);
	}
	bzero(&addr,sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(SERVER_PORT);
	/*bind 函数*/
	if(bind(sockfd,(struct sockaddr*)&addr,sizeof(struct sockaddr_in)) < 0) {
	fprintf(stderr,"Bind Error:%s\n",strerror(errno));
	exit(1);
}
	struct sockaddr_in client_addr;
	int n = 0;
	char msg[MAX_MSG_SIZE] ;
	socklen_t len = sizeof(client_addr);
	puts("server are receiving ......");
	while(1) {
		/*从网络上读，写到网络上面去*/
		memset(msg,0,sizeof(msg));
		if((n = recvfrom(sockfd,msg,MAX_MSG_SIZE,0,(struct sockaddr*)&client_addr, &len)) == -1) {
			fprintf(stderr,"recv error:%s\n",strerror(errno));
			exit(1);
		}
	//	puts("recv success");
		fprintf(stdout,"i have received %s",msg);
	//	puts("answer to client");
		ret = sendto(sockfd,msg,n,0,(struct sockaddr*)&client_addr,sizeof(client_addr));
		if (ret == -1) {
			fprintf(stderr,"sendto error%s\n",strerror(errno));
			exit(1);
		}
		//printf("msg = %s\n",msg);
	//	puts("answer complete");
	}
	close(sockfd);
}
