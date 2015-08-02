#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
	pid_t childpid;
	int fd[2] ,nbytes;
	char string[]="data from child process\n";
	char buf[100] = {0};
	if(pipe(fd)<0) {
		perror("pipe");
		exit(1);
	}

	if((childpid=fork()) == -1) {
		perror("fork");
		exit(1);
	}

	if(childpid == 0) {
		close(fd[0]);
		printf("childpid=%2d\n",getpid());
		write(fd[1],string,strlen(string));
		exit(0);
	} else {
		close(fd[1]);
		printf("parentpid=%2d\n",getppid());
		nbytes=read(fd[0],buf,strlen(string));
		printf("Received string:%s\n",buf);
	}
	return 0;
}
