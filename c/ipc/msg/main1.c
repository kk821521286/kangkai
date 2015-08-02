#include "msg.h"

int main()
{	
	int running = 1;
	struct my_msg_st some_data;
	int msgid;
	char buffer[BUFSIZ];
	msgid = msgget((key_t)1234,0666 | IPC_CREAT);
	if(msgid == -1) {
		fprintf(stderr,"msgget failed with error: %d\n",errno);
		exit(EXIT_FAILURE);
	}

	while(running) {
		printf("Enter some text: ");
		fgets(buffer,BUFSIZ,stdin);
		some_data.my_msg_type = 3;
		strcpy(some_data.some_text,buffer);

		if (msgsnd(msgid,(void *)&some_data,MAX_TEXT,0) == -1) {
			fprintf(stderr,"msgsnd failed\n");
			exit(EXIT_FAILURE);
		}
		if	(strncmp(buffer,"end",3) == 0) {
			running = 0;
		}
	}
	return 0;
}
