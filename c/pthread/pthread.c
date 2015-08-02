#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t mutex;
pthread_cond_t cond_full;
pthread_cond_t cond_empty;

int g_iBuffSize = 0;

void *thread_producer(void * arg)
{
	while(1)
	{
		printf("thread_producer:pthread_mutex_lock\n");
		pthread_mutex_lock(&mutex);
		sleep(1);
		
		if(0 != g_iBuffSize)
		{
			printf("thread_producer: \
			pthread_cond_wait cond_empty \n");
			pthread_cond_wait(&cond_empty,&mutex);
		}

		printf("thread_producer>>>>>>\n");
		g_iBuffSize = 1;

		printf("thread_producer: pthread_cond_signal\n");
		pthread_cond_signal(&cond_full);
		pthread_mutex_unlock(&mutex);
	}
	return NULL;
}


void * thread_consumer(void * arg)
{
	while(1)
	{
		printf("thread_consumer:pthread_mutex_lock\n");
		pthread_mutex_lock(&mutex);
		sleep(1);
		if(0 == g_iBuffSize)
		{
			printf("thread_consumer: pthread_cond_wait\n");
			pthread_cond_wait(&cond_full,&mutex);
		}

		printf("thread_consumer>>>\n");
		g_iBuffSize = 0; 
	
		printf("thread_consumer: pthread_cond_signal\n");
		pthread_cond_signal(&cond_empty);

		pthread_mutex_unlock(&mutex);
	}

	return NULL;
}

int main()
{
	void * retval1,*retval2;
	pthread_t thread_id_1,thread_id_2;

	pthread_mutex_init(&mutex,NULL);
	pthread_cond_init(&cond_full,NULL);
	pthread_cond_init(&cond_empty,NULL);

	pthread_cond_signal(&cond_empty);
	pthread_create(&thread_id_1,NULL,thread_producer,NULL);
	pthread_create(&thread_id_2,NULL,thread_consumer,NULL);
	
	pthread_join(thread_id_1,&retval1);
	pthread_join(thread_id_2,&retval2);

	pthread_cond_destroy(&cond_full);
	pthread_cond_destroy(&cond_empty);
	pthread_mutex_destroy(&mutex);




	return 0;
}
