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


bool comp(int a, int b)
{
	return a>b?1:0;
}


int main(int argc, char** argv) 
{
int N;
N= atoi(argv[1]);

    int pipefd[2];
           pid_t cpid;
           int buf;
	srand(120);
	int fd;
	size_t size;
	char name[]="my.fifo";
	(void)umask(0);
	if(mknod(name, S_IFIFO|0666,0)<0)		//создаем FIFO
	{
		printf("Cant FIFO");
		exit(-1);
	}
           if (argc != 2) {
               fprintf(stderr, "Usage: %s <string>\n", argv[0]);
               exit(EXIT_FAILURE);
           }

           if (pipe(pipefd) == -1) {	//создаем pipe
               perror("pipe");
               exit(EXIT_FAILURE);
           }

           cpid = fork(); //попрождаем второй процесс
           if (cpid == -1) {
               perror("fork");
               exit(EXIT_FAILURE);
           }

           if (cpid == 0) {   
               close(pipefd[1]);        
		int mes[1];
		int mas[N];
		int k=0;
               while (read(pipefd[0], mes, 4) > 0) //получаем числа через pipe
		{
		   mas[k]=mes[0];
		//   printf("%d",mas[k]);
		   k++;
		}
		std::sort(mas, mas+N, comp); //сортировка
		//for (int i =0; i<N; i++)
		//	printf("%d",mas[i]);
               close(pipefd[0]);
		if((fd=open(name, O_WRONLY))<0)
		{
			printf("Cant FIFO");
			exit(-1);
		}
		//for (int i=0; i<N; i++)
		//{
			//mes[0]=mas[k];
			write(fd, mas, 4*N); //передаем массив с отсортированными числами через FIFO
		//}
		close(fd);
		exit(0);

           } else {            
               close(pipefd[0]);         
		for (int i=0; i<N; i++) //генирируем цисла и передаем их через pipe
		{
		int s[1];
		s[0] =rand()%10;
                write(pipefd[1], s, 4);   
		}
               close(pipefd[1]);        
		
		if((fd=open(name, O_RDONLY))<0)
		{
			printf("Cant FIFO");
			exit(-1);
		}
		wait(NULL); 
		int s[1];
		while (read(fd, s, 4) > 0)	//получаем отсортированные числа и выводим их на экран
		{
			printf ("%d\n", s[0]);
		}
		close(fd);
		unlink(name);

               exit(0);
           }
} 

