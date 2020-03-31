#include <stdio.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include <algorithm>
#include <sys/stat.h>
#include <sys/sem.h>

void initSemValue(int semId, int n)
{
    struct sembuf op;
    op.sem_op = 1;
    op.sem_flg = 0;
    op.sem_num = n;
    semop(semId, &op, 1);
}

void tryToGetSemOrWait(int semId, int n)
{
    struct sembuf op;
    op.sem_op = -1;
    op.sem_flg = 0;
    op.sem_num = n;
    semop(semId, &op, 1);
}

void releaseSem(int semId, int n)
{
    initSemValue(semId, n);
}



int main(int argc, char** argv) 
{
int N;
N= atoi(argv[1]);
int min = atoi(argv[2]);
int max = atoi(argv[3]);
int memId = shmget(IPC_PRIVATE, 4*N, 0600|IPC_CREAT|IPC_EXCL);
if (memId <= 0)
{
	printf("error with shmget()\n");
	return -1;
}
int *mem = (int *)shmat(memId, 0, 0);
if (NULL == mem)
{
	printf("error with shmat()\n");
	return -2;
}
for (int i=0; i<N; i++)
{
	mem[i]=rand()%(max-min+1)+min;
	printf("%d\n",mem[i]);
}
const size_t semCount = N;
    int semId = semget(IPC_PRIVATE, semCount, 0600|IPC_CREAT);
    if (semId < 0)
    {
        perror("error with semget()");
        return -1;
    }
    else
    {
        printf("semaphore set id = %i\n", semId);
    }


	pid_t childId = fork();
    if (childId < 0)
    {
        perror("error with fork()\n");
    }
    else if (childId > 0)
    {
	for (int i = 0; i < N; ++i)
        {
            printf("Parent: Freeing semaphore %i\n", i);
            releaseSem(semId, i);
        }
	int status=0;
        while (waitpid(-1, &status, 0|WNOHANG)==0)
	{
	for(int i=0; i<N; i++)
	{
		tryToGetSemOrWait(semId, i);
		printf("%d\n", mem[i]);
		releaseSem(semId, i);
	}
	}
	printf("Sort is end\n");
	for(int i=0; i<N; i++)
	{
		tryToGetSemOrWait(semId, i);
		printf("%d\n", mem[i]);
		releaseSem(semId, i);
	}
	shmctl(memId, IPC_RMID, NULL);
	semctl (semId, IPC_RMID, 0);
    }
    else
    {
        int key =0;
	int i=0;
	for(int j=1;j<N;j++){
	tryToGetSemOrWait(semId, j);
	key = mem[j];
	releaseSem(semId, j);
	i=j-1;
	while(i>=0 && mem[i]>key){
	tryToGetSemOrWait(semId, i);
	tryToGetSemOrWait(semId, i+1);
	mem[i+1]=mem[i];
	releaseSem(semId, i);
	releaseSem(semId, i+1);
	i=i-1;
	tryToGetSemOrWait(semId, i+1);
	mem[i+1]=key;
	releaseSem(semId, i+1);
	}
	}
	exit(0);
    }

}
