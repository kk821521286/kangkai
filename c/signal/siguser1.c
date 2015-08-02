#include <stdio.h>
#include <unistd.h>
#include <signal.h>
callback(void)
{
	printf("signaluse1 calling ......\n");
}
int main(void)
{
	while(1) {
		signal(SIGUSR1,callback);
		sleep(1);
	};
	return 0;
}
