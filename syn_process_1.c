#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include "display.h"


int main(int argc, char **argv)
{
	int my_sem, semDestruction;

	struct sembuf up = {0,1,0};
	struct sembuf down = {0,-1,0};

	my_sem = semget (IPC_PRIVATE,1,0600);

	union semun
	{
	      int val;
	      struct semid_ds *buf;
	      ushort *array;
	};

	union semun arg;
	arg.val = 1;
	semctl(my_sem, 0, SETVAL, arg);

	if (fork())
	{
		for (int i=0; i<10; i++)
		{
			semop(my_sem,&down,1);
			display("Hello world\n");
			semop(my_sem,&up,1);
		}
		wait(NULL);
	}
	else
	{
		for (int i=0; i<10; i++)
		{
			semop(my_sem,&down,1);
			display("Kalimera kosme\n");
			semop(my_sem,&up,1);
		}
	}
	semDestruction = semctl(my_sem,0,IPC_RMID);
	return 0;
}
