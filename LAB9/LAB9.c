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
    char *filename = argv[1];
    int size = atoi(argv[2]);
    FILE *fin = fopen(filename, "r");
    if (NULL == fin)
    {
        perror("cannot open file");
        return -1;
    }
    
    int data[size];
    int res = fread(data, sizeof(int), size, fin);
    printf("fread result = %i\n", res);
	for (int i=0; i<res; i++)
	{
    printf("random number = %i (0x%x)\n", data[i], data[i]);
	}
    
    fclose(fin);
    return 0;
}
