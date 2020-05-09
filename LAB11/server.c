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

// Driver code 
int main(int argc, char** argv) {
	int porti = atoi(argv[1]); 
    int sockfd; 
    int buffer[1000]; 
    char *hello = "Hello from server"; 
    struct sockaddr_in servaddr, cliaddr; 
      
    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
      
    memset(&servaddr, 0, sizeof(servaddr)); 
    memset(&cliaddr, 0, sizeof(cliaddr)); 
      
    // Filling server information 
    servaddr.sin_family    = AF_INET; // IPv4 
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    servaddr.sin_port = htons(porti); 
      
    // Bind the socket with the server address 
    if ( bind(sockfd, (const struct sockaddr *)&servaddr,  
            sizeof(servaddr)) < 0 ) 
    { 	
	servaddr.sin_port = 0; 
	if ( bind(sockfd, (const struct sockaddr *)&servaddr,  
            sizeof(servaddr)) < 0 )
{
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
}
    } 
	printf("Server start on port: %d\n", ntohs(servaddr.sin_port));
    
    int len, n; 
  
    len = sizeof(cliaddr);  //len is value/resuslt 
  
    n = recvfrom(sockfd, (int *)buffer, 1000,  
                MSG_WAITALL, ( struct sockaddr *) &cliaddr, 
                &len); 
    int k=0;
	while (buffer[k]!=-9999)
	{
		printf("%d\n", buffer[k]);
		k++;
	} 
	int NUM = buffer[0];
		for(int i = 2; i < NUM+1; ++i)
	{
		for(int r = 1; r < NUM+2-i; r++)
		{
			if(buffer[r] < buffer[r+1])
			{
				// Обмен местами
				int temp = buffer[r];
				buffer[r] = buffer[r+1];
				buffer[r+1] = temp;
			}
		}
	}

   sendto(sockfd, (const int *)buffer, (NUM+2)*sizeof(int),  
        MSG_CONFIRM, (const struct sockaddr *) &cliaddr, 
            len); 
    printf("Sort sent.\n");  
      
    return 0; 
} 
