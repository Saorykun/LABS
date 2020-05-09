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
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

	int main(int argc, char *argv[]) {
	    int listenfd = 0, connfd = 0;
	    struct sockaddr_in serv_addr;
	    char sendBuff[1025];
	    time_t ticks;
	    listenfd = socket(AF_INET, SOCK_STREAM, 0);
	    memset(&serv_addr, '0', sizeof(serv_addr));
	    memset(sendBuff, '0', sizeof(sendBuff));

	    serv_addr.sin_family = AF_INET;
	    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	    serv_addr.sin_port = htons(atoi(argv[1]));
	    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	    listen(listenfd, 10);

	    while(1) {
	        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
		printf("+\n");
		pid_t childId = fork();
   		if (childId < 0)
   		{
   		     perror("error with fork()\n");
    		}
    		else if (childId > 0)
   		 {
			printf("connect\n");
			printf("+\n");
        		close(connfd);
   		 }
   		 else
   		 {
			while (1)
			{
			printf("+\n");
    			int sizef;
			char namef[1000];
			read (connfd, namef, 1000*sizeof(char));
			printf("%s\n", namef);
			if (namef[0]=='.')
			{
				printf("close\n");
				close(connfd);
				exit(0);
			}
			int fmapd = open(namef, O_RDWR);
       			if (fmapd < 0)
        		{
				int no[1];
				no[0] = htons(0);
				write (connfd, no, sizeof(int));
        			perror("error with open of map file\n");
       				continue;
       			}
			printf("%d\n", fmapd);
			struct stat *stat1;
			fstat (fmapd, stat1);
			int maxdatalength[1];
			maxdatalength[0] = stat1->st_size;
			//printf ("%d\n", maxdatalength[0]);
			char *data = (char *)mmap(NULL, maxdatalength[0], PROT_WRITE | PROT_READ, MAP_SHARED, fmapd, 0);
    			if (MAP_FAILED == data)
  			{
       				 perror("error with mmap");
       				 return -2;
    			}
			
			
			maxdatalength[0] = htons(maxdatalength[0]);
			write (connfd, maxdatalength, sizeof(int));
			printf ("%d\n", maxdatalength[0]);
			write (connfd, data, maxdatalength[0]);
			printf("+\n");
			close (fmapd);
			}
			
   		 }
		
	     }
	}
