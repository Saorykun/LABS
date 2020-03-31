#include <stdio.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void *allocateSharedMemory(size_t memSize, int &memId)
{
    memId = shmget(IPC_PRIVATE, memSize, 0600|IPC_CREAT|IPC_EXCL);
    if (memId <= 0)
    {
        perror("error with memId");
        return NULL;
    }
    void *mem = shmat(memId, 0, 0);
    if (NULL == mem)
    {
        perror("error with shmat");
    }
    return mem;
}

void childMainCode(int *sharedMem) //Сортируем и выводим на экран
{
        int key =0;
    int i=0;
    for(int j=1;j<20;j++){
        key = sharedMem[j];
        i=j-1;
        while(i>=0 && sharedMem[i]>key){
            sharedMem[i+1]=sharedMem[i];
            i=i-1;
            sharedMem[i+1]=key;
        }
    }

    for(int i=0;i<20;i++)
    {   
        printf("%d\n", sharedMem[i]);
    }   
}

int main()
{
    int memId;
    int *sharedMem = (int *)allocateSharedMemory(1024, memId);

    printf("memId = %d\n", memId);

    printf("starting child process...\n");
    pid_t childId = fork();
    //делаем копию процесса
    if (childId < 0)
    {
        perror("error with fork()\n");
    }
    else if (childId > 0)
    {
        printf("Parent: This is main process. Child id = %d\n", childId);
        printf("Parent: awaiting for child.\n");
    int a[20];
    for (int i=0; i<20; i++)
        sharedMem[i]=rand()%100; //Формируем массив случайных чисел
        waitpid(childId, NULL, 0); //Ждем завершения дочернего процесса
    shmdt(sharedMem); //удаляем разделенную память
    printf("Shared memory is clear.\n");
    }
    else
    {
        childMainCode(sharedMem);    
    }
    pid_t curId = getpid();
    printf("%6d proc: Process is finished.\n", curId);
    
    return 0;
}
