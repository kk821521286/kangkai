#include "fifo_cs.h"

int main()
{
	int serverfd,clientfd;
	pid_t serverpid;
	char client_name[30];
	struct linking slinker,clinker;

	printf("Input Information\n");
	printf("The server name information\n");
	scanf("%s",slinker.name);

	mkfifo(SERVER_FIFO_NAME,0660);

	if((serverfd = open(SERVER_FIFO_NAME,O_RDWR)) == -1) {
		printf("Server can't open\n");
		fflush(stdout);
		exit(1);
	}

	serverpid = getpid();
	slinker.pid = serverpid;

	while(1) {
		read(serverfd,&clinker,sizeof(struct linking));
		sprintf(client_name,CLIENT_FIFO_NAME,clinker.pid);
		printf("Server:Get client pid:%3d\n",clinker.pid);
		printf("Server:Get client name:%s\n",clinker.name);

		if((clientfd = open(client_name,O_WRONLY)) == -1) 
			continue;
		write(clientfd,&slinker,sizeof(slinker));
		close(clientfd);
	}
}
