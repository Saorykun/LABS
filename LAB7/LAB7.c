#include <stdio.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <sys/ipc.h>

int comp (const int *i, const int *j)
{
	return *i-*j;
} 

void swap(int *a, int i, int j)
{
  int s = a[i];
  a[i] = a[j];
  a[j] = s;
}

int NextSet(int *a, int n)
{
  int j = n - 2;
  while (j != -1 && a[j] >= a[j + 1]) j--;
  if (j == -1)
    return 0; 
  int k = n - 1;
  while (a[j] >= a[k]) k--;
  swap(a, j, k);
  int l = j + 1, r = n - 1; 
  while (l<r)
    swap(a, l++, r--);
  return 1;
}

struct Strmsg
{
    long mtype;
    int mas[4];
};

void* treat (void* Id)
{
	int* msg = (int*)Id;
	int msgId = msg[0];
	struct Strmsg localmsg;
	ssize_t len = msgrcv(msgId, &localmsg, 16, 12, 0);
	//printf ("%d", localmsg.mas[0]);
	localmsg.mtype = 101;
	msgsnd(msgId, &localmsg, 16, 0);
	localmsg.mtype = 12;
	msgsnd(msgId, &localmsg, 16, 0);
	qsort(localmsg.mas, 4, sizeof(int), (int(*)(const void*,const void*)) comp);
	while (NextSet(localmsg.mas, 4))
	{
    		localmsg.mtype = 12;
		msgsnd(msgId, &localmsg, 16, 0);
	}
	localmsg.mas[0] = -1;
	localmsg.mtype = 12;
	msgsnd(msgId, &localmsg, 16, 0);
	
}

int main () {
int msgId = msgget(IPC_PRIVATE, 0600|IPC_CREAT);
    if (msgId < 0)
    {
        perror("error with msgget()");
        return -1;
    }
    else
    {
        printf("message id = %i\n", msgId);
    }
	struct Strmsg localmsg;
    int i;
    int mas[4];
    for (i = 0; i < 4; ++i)
    {
	localmsg.mas[i] = rand()%10;
    }
        localmsg.mtype = 12;
        msgsnd(msgId, &localmsg, 16, 0);
	pthread_t thread1;
	int msg[1];
	msg[0]=msgId;
	const int *Id = msg;
	int res1 = pthread_create(&thread1, NULL, treat, (void*)Id);
	int iret1;
	pthread_join(thread1, (void **)&iret1);
	ssize_t len = msgrcv(msgId, &localmsg, 16, 101, 0);
	int check=0;
	do {
		ssize_t len = msgrcv(msgId, &localmsg, 16, 12, 0);
		if (localmsg.mas[0]!=-1)
		{
			for (int i =0; i<4; i++)
			{
				printf("%d ", localmsg.mas[i]);
			}
			printf("\n");
			check++;
		}
	}while(localmsg.mas[0]!=-1);
	printf("%d\n", check);
	
	msgctl(msgId, IPC_RMID, NULL);
}


