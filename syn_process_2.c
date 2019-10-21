#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "display.h"
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/shm.h>


int main(int argc, char **argv)
{
	int my_sem, shmid, *shMemory, semDestruction;

	struct sembuf up = { 0,1,0 };
	struct sembuf down = { 0,-1,0 };

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

	shmid = shmget(IPC_PRIVATE,sizeof(int),0600);
	shMemory = (int*)shmat(shmid,0,0);

	if (fork())
	{
		for (int i=0; i<10; i++)
		{
			semop(my_sem,&down,1);
			display("ab");
			*shMemory = 1;
			semop(my_sem,&up,1);
			do
				sleep(1);
			while (*shMemory != 0);
			}
			wait(NULL);
		 }
	else
	{
		for (int i=0; i<10; i++)
		{
			semop(my_sem,&down,1);
			display("cd\n");
			*shMemory = 0;
			semop(my_sem,&up,1);
			do
				sleep(1);
			while (*shMemory!=1 && i!=9);
			}
	 }
	 shmdt((void*)shMemory);
	 shmctl(shmid,IPC_RMID,0);
	 semDestruction = semctl(my_sem,0,IPC_RMID);

	 return 0;
}
