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
	    int sockfd = 0, n = 0;
	    char recvBuff[1024];
	    struct sockaddr_in serv_addr;
	    if(argc != 3) {
	        printf("\n Usage: %s <ip of server> \n",argv[0]);
	        return 1;
	    }
	    memset(recvBuff, '0',sizeof(recvBuff));
	    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	    {
	        printf("\n Error : Could not create socket \n");
	        return 1;
	    }
	    memset(&serv_addr, '0', sizeof(serv_addr));
	    serv_addr.sin_family = AF_INET;
	    serv_addr.sin_port = htons(atoi(argv[2]));
	    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
	    {
	        printf("\n inet_pton error occured\n");
	        return 1;
	    }
	 printf("+\n");
	    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	    {
	       printf("\n Error : Connect Failed \n");
	       return 1;
	    }
	printf("+\n");
	
	 while (1)
	{
	int sizef[1];
	char namef[1000];
	scanf ("%s", namef);
	printf("Scanf\n");
	if (namef[0]=='.') break;
	printf("+\n");
	write(sockfd, namef, 1000*sizeof(char));
	sleep(1);
	read(sockfd, &sizef[0], sizeof(int));
	sizef[0] = ntohs(sizef[0]);
	printf ("%d\n", sizef[0]);
	if (sizef[0]==0)
	{
		printf ("No file: %s\n", namef);
		continue;
	}
	//else 
	//{
		//sizef[0] = ntohs(sizef[0]);
		printf ("%d\n", sizef[0]);
		char file[sizef[0]];
		read(sockfd, file, sizef[0]);
		printf ("%s\n", file);
		int fmapd = open(namef, O_RDWR | O_CREAT, 0600);
		if (fmapd < 0)
		{
				int no[1];
				no[0] = -1;
				write (sockfd, no, sizeof(int));
        			perror("error with open of map file\n");
       				exit(-1);
       		}
		write (fmapd, file, sizef[0]);
		ftruncate(fmapd, sizef[0]);
		close (fmapd);
	//}
	   
	}
	char b[] = ".";
	write(sockfd, b, 1000*sizeof(char));
	close(sockfd);
	    return 0;
	}
