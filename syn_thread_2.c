#include <stdio.h>
#include "display.h"
#include <pthread.h>

/******  Function Prototypes  ******/
void* disp1(void* );
void* disp2(void* );

/*** Mutex Declaration ***/
pthread_mutex_t mutex;

pthread_cond_t cond_var;
int predicate = 0;

int main(int argc, char **argv)
{
	int tmp;
	tmp = pthread_mutex_init(&mutex, NULL);

	pthread_t th1,th2;

	pthread_create(&th1,NULL,disp1,"ab");
	pthread_create(&th2,NULL,disp2,"cd\n");

	pthread_join(th1,NULL);
	pthread_join(th2,NULL);

	pthread_mutex_destroy(&mutex);
	return 0;
}

/******  Function Definitions  ******/
void* disp1(void* output)
{
	for (int i=0; i<10; i++)
	{
		pthread_mutex_lock(&mutex);
		while(predicate != 0)
		  pthread_cond_wait(&cond_var,&mutex);
		pthread_mutex_unlock(&mutex);

		display(output);

		pthread_mutex_lock(&mutex);
		predicate = 1;
		pthread_cond_signal(&cond_var);
		pthread_mutex_unlock(&mutex);
	 }
	pthread_exit(NULL);
}

void* disp2(void* output)
{
	for (int i=0; i<10; i++)
	{
		pthread_mutex_lock(&mutex);
		while(predicate != 1)
		  pthread_cond_wait(&cond_var,&mutex);
		predicate = 1;
		pthread_mutex_unlock(&mutex);

		display(output);

		pthread_mutex_lock(&mutex);
		predicate = 0;
		pthread_cond_signal(&cond_var);
		pthread_mutex_unlock(&mutex);
	 }
	pthread_exit(NULL);
}
