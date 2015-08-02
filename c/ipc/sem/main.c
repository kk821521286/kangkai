#include "head.h"
#include "sem.h"


//主函数
int main(int argc,char **argv)
{
	int i;
	int pause_time;
	char op_char = 'O';
	srand((unsigned int)getpid());
	sem_id = semget((key_t)1234,1,0666 | IPC_CREAT);

	if(argc > 1)
	{
		if(!set_semvalue())
		{
			fprintf(stderr,"Failed to initialize semaphore\n");
			exit(EXIT_FAILURE);
		}
		op_char = 'X';
		sleep(2);
	}
		if(!set_semvalue())
		{
			fprintf(stderr,"Failed to initialize semaphore\n");
			exit(EXIT_FAILURE);
		}
	
	for(i = 0; i < 10; i++) {

		if(!semaphore_p()) exit(EXIT_FAILURE);
		printf("%c",op_char); fflush(stdout);
		pause_time = rand() % 3;
		sleep(pause_time);

		if(op_char == 'X') 
			op_char = 'O';
		else 
			op_char = 'X';
		printf("%c",op_char); fflush(stdout);

		if(!semaphore_v()) exit(EXIT_FAILURE);
		pause_time = rand() % 2;
		sleep(pause_time);
	}

	printf("\n%d-finished\n",getpid());

	if(argc > 1) {
		sleep(10);
		del_semvalue();
	}

	exit(EXIT_FAILURE);
	return 0;
}

