#include "msg.h"

int main()
{
	int running = 1;
	int msgid;
	struct my_msg_st some_data;
	long int msg_to_receive = 3;

	//get msgid 
	msgid = msgget((key_t)1234,0666 | IPC_CREAT);

	if(msgid == -1) {
		fprintf(stderr,"msgget failed with error:%d\n",errno);
		exit(EXIT_FAILURE);
	}

	// 1.receive the message from the msg queue
	// 2.until the "end" message is occured
	// 3.finally ,the message queue is deleted

	while(running) {
		if(msgrcv(msgid,(void*)&some_data,BUFSIZ,msg_to_receive, 0) == -1) {
			fprintf(stderr,"msgrcv failed with error:%d\n",errno);
			exit(EXIT_FAILURE);
		}
		printf("You wrote :%s",some_data.some_text);
		if(strncmp(some_data.some_text,"end",3) == 0) {
			running = 0;
		}
	}
#if 0
	if (msgctl(msgid,IPC_RMID,0) == -1) {
		fprintf(stderr,"msgctl(IPC_RMID) failed\n");
		exit(EXIT_FAILURE);
	}
#endif
	exit(EXIT_SUCCESS);
}



















