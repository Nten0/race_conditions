#include <stdio.h>
#include "display.h"
#include <pthread.h>
#include <unistd.h>

/******  Function Prototypes  ******/
void* disp(void* );

/*** Mutex Declaration ***/
pthread_mutex_t mutex;

int main(int argc, char **argv)
{
	int tmp;
	tmp = pthread_mutex_init(&mutex,NULL);

	pthread_t th1,th2;

	pthread_create(&th1,NULL,disp,"Hello world\n");
	pthread_create(&th2,NULL,disp,"Kalimera kosme\n");

	pthread_join(th1,NULL);
	pthread_join(th2,NULL);

	pthread_mutex_destroy(&mutex);
	return 0;
}

/******  Funtion Definitions  ******/
void* disp(void* output)
{
	for (int i=0; i<10; i++)
	{
		pthread_mutex_lock(&mutex);
		display(output);
		pthread_mutex_unlock(&mutex);
	 }
	pthread_exit(NULL);
}
