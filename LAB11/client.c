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

int main(int argc, char** argv)  { 
    char* addr = argv[1];	
    int i=0;
    int porti = atoi(argv[2]);
    int sockfd; 
    int buffer[1000]; 
    char hello[1000] = "Hello from client"; 
    struct sockaddr_in     servaddr; 
  
    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
  
    memset(&servaddr, 0, sizeof(servaddr)); 
      
    // Filling server information 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(porti); 
    servaddr.sin_addr.s_addr = inet_addr(addr); 
      int NUM, MAXI, MINI;
	scanf("%d", &NUM);
	scanf("%d", &MAXI);
	scanf("%d", &MINI);
	int mas[NUM+2];
	mas[0]=NUM;
	for (int i=1; i<=NUM; i++)
	{
		mas[i]=rand()%(MAXI-MINI)+MINI;
	}
	mas[NUM+1] = -9999; 
    int n, len; 
      
    sendto(sockfd, (const int *)mas, (NUM+2)*sizeof(int), 
        MSG_CONFIRM, (const struct sockaddr *) &servaddr,  
            sizeof(servaddr)); 
    printf("Numbers sent.\n"); 
          
    n = recvfrom(sockfd, (int *)buffer, 1000,  
                MSG_WAITALL, (struct sockaddr *) &servaddr, 
                &len); 
	int k=1;
	while (buffer[k]!=-9999)
	{
		printf("%d\n", buffer[k]);
		k++;
	} 
  
    close(sockfd); 
    return 0; 
} 
