#include "fifo_cs.h"

int main()
{
	int serverfd,clientfd;
	pid_t clientpid;
	char client_name[30];
	struct linking clinker,slinker;

	printf("Init Information\n");
	printf("The client name information\n");
	scanf("%s",clinker.name);

	if((serverfd = open(SERVER_FIFO_NAME,O_WRONLY)) == -1) {
		printf("Server:can't open\n");
		exit(1);
	}

	clientpid = getpid();
	clinker.pid = clientpid;
	sprintf(client_name,CLIENT_FIFO_NAME,clinker.pid);
	mkfifo(client_name,0660);

	if((clientfd = open(client_name,O_RDWR)) == -1) {
		close(serverfd);
		printf("Can't open fifo\n");
		exit(1);
	}

	write(serverfd,&clinker,sizeof(struct linking));
	read(clientfd,&slinker,sizeof(struct linking));
	printf("Client:Get server pid:%3d\n",slinker.pid);
	printf("Client:Get server name:%s\n",slinker.name);

	close(serverfd);
	close(clientfd);
	unlink(client_name);

	return 0;
}
