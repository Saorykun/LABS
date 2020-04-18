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
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <signal.h>

int i1 = 0, i2 =0, i3=0;
void handl_int(int nsig)
{
    printf("received signal: %d\n", nsig);
    i1++;	
}
void handl_int1(int nsig)
{
    printf("received signal: %d\n", nsig);
    i2++;
}
void handl_int2(int nsig)
{
    printf("received signal: %d\n", nsig);
    i3++;
}
int main()
{
    printf("%d\n", getpid());
    signal(SIGINT, handl_int);
    signal(SIGUSR1, handl_int1);
    signal(SIGUSR2, handl_int2);
    (void)signal(SIGTERM, SIG_IGN);
    for(;;)
    {
	time_t seconds = time(NULL);
	struct tm* timeinfo = localtime(&seconds);
	printf(asctime(timeinfo));
	printf("SIGINT: %d\n",i1);
	printf("SIGUSR1: %d\n",i2);
	printf("SIGUSR2: %d\n",i3);
	if (i1==5)
	{
		printf("FINAL:\n");
		printf("SIGINT: %d\n",i1);
		printf("SIGUSR1: %d\n",i2);
		printf("SIGUSR2: %d\n",i3);
		return 0;
	}
        sleep(1);
    }
    return 0;
}

