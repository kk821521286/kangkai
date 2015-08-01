#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	int pfds[2];
	if(pipe(pfds)==0) {
		
		if(fork()==0) {
			close(1);//关闭stdout描述符
			dup2(pfds[1],1);//把stdout重定向到管道(pfds[1])
			close(pfds[0]);
			execlp("ls","ls","-l",NULL);//把子进程的映像替换为ls -l的进程映像
		}
		else {
		
			close(0);//关闭stdin描述符
			dup2(pfds[0],0);//让stdin变成管道的输出端
			close(pfds[1]);//关闭管道的stdout端(pfds[1])
			execlp("wc","wc","-l",NULL);//把父进程的命令替换为命令wc -l 的进程映像 
		}
	}
	return 0;
}
