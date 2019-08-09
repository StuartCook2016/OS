#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#include "shm.h"
#include "sem.h"

int main()
{
    system("clear");

    FILE * fp;
    int shmFd, shmFd2, shmFd3;
    SHMstruct initData = { 1, false, false, false};
    SHMstruct2 initData2 = { 1, false, false, false};
    SHMstruct3 initData3 = { 1, false, false, false};
    SHMstruct * shmData;
    SHMstruct2 * shmData2;
    SHMstruct3 * shmData3;
    sem_t *semaphore;

    fp = fopen("/tmp/server.log", "a");
    if(fp == NULL) 
    {
        perror("fopen");
        return EXIT_FAILURE;
    }
    fprintf(fp,"Log file opened \n");

    fprintf(stderr, "Shared Memory Area created\n");
    
    shmFd = createSHM(SHNAME);
    shmData = initSHM( shmFd, &initData );
    shmData2 = initSHM( shmFd, &initData2 );
    shmData3 = initSHM( shmFd, &initData3 );

    fprintf(fp, "Shared Memory Area created\n");

    semaphore = createSem(SEMNAME,1);

    fclose(fp);
    fp = fopen("/tmp/server.log", "a");

    printf("creating new ticket %d in channel 1\n", shmData->ticket);
    fprintf(fp, "creating new ticket %d in channel 1\n", shmData->ticket);
    printf("creating new ticket %d in channel 2\n", shmData->ticket);
    fprintf(fp, "creating new ticket %d in channel 2\n", shmData->ticket);
    printf("creating new ticket %d in channel 3\n", shmData->ticket);
    fprintf(fp, "creating new ticket %d in channel 3\n", shmData->ticket);

   while(!(shmData->soldOut) && !(shmData2->soldOut) && !(shmData2->soldOut))
    {
      lockSem(semaphore);
      if(shmData->isTaken==true)
      {
        if(!(shmData->ticket==MAX_TICKETS))
        {
          shmData->ticket++;
          printf("creating new ticket %d in channel 1 \n", shmData->ticket);
          fprintf(fp, "creating new ticket %d in channel 1\n", shmData->ticket);
          shmData->isTaken= false;
        }
        else if(shmData->ticket==MAX_TICKETS)
        {
          fprintf(fp, "tickets sold out in channel 1\n");
          printf("tickets sold out in channel 1\n");
          shmData->soldOut= true;
        }
      }

      if(shmData2->isTaken==true)
      {
        if(!(shmData2->ticket==MAX_TICKETS))
        {
          shmData2->ticket++;
          printf("creating new ticket %d in channel 2 \n", shmData2->ticket);
          fprintf(fp, "creating new ticket %d in channel 2\n", shmData2->ticket);
          shmData2->isTaken= false;
        }
        else if(shmData2->ticket==MAX_TICKETS)
        {
          fprintf(fp, "tickets sold out in channel 2\n");
          printf("tickets sold out in channel 2\n");
          shmData2->soldOut= true;
        }
      }

      unlockSem(semaphore);
    }
    closeSem(semaphore);
    destroySem(SEMNAME);

    fprintf(stderr, "Shared Memory Area destroyed\n");
    clearSHM(shmData);
    closeSHM(shmFd);
    destroySHM(SHNAME);
    fprintf(fp, "closing log file\n");
    fclose(fp);

    return EXIT_SUCCESS;
}