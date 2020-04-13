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


int main(int argc, char** argv) 
{
	char* from = argv[1];
	char* to = argv[2];
	int fmapd = open(from, O_RDWR | O_CREAT, 0600);
        if (fmapd < 0)
        {
        	perror("error with open of map file");
       		return -1;
        }
	int nfmapd = open(to, O_RDWR | O_CREAT, 0600);
        if (nfmapd < 0)
        {
        	perror("error with open of map file");
       		return -1;
        }
	struct stat *stat1;
	fstat (fmapd, stat1);
	int maxdatalength = stat1->st_size;
	char *data = (char *)mmap(NULL, maxdatalength, PROT_WRITE | PROT_READ, MAP_SHARED, fmapd, 0);
    if (MAP_FAILED == data)
    {
        perror("error with mmap");
        return -2;
    }
	lseek (nfmapd, 0, SEEK_SET);
	write (nfmapd, data, maxdatalength);
	ftruncate(nfmapd, maxdatalength);
	close (fmapd);
	close (nfmapd);
	
}
