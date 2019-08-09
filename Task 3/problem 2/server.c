#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#include "shm.h"
#include "sem.h"

int main()
{
    system("clear");

    FILE * fp;
    int shmFd;
    SHMstruct initData = { 1, false, false};
    SHMstruct * shmData;
    sem_t *semaphore;

    fp = fopen("/tmp/server.log", "a");
    if(fp == NULL)
    {
        perror("fopen");
        return EXIT_FAILURE;
    }

    fprintf(fp,"Log file opened | Time: %s \n", getTimeStamp());

    fprintf(stderr, "Shared Memory Area created | Time: %s \n", getTimeStamp());

    shmFd = createSHM(SHNAME);
    shmData = initSHM( shmFd, &initData );

    fprintf(fp, "Shared Memory Area created | Time: %s \n", getTimeStamp());

    semaphore = createSem(SEMNAME,1);

    fclose(fp);
    fp = fopen("/tmp/server.log", "a");

    printf("creating new ticket %d | Time: %s \n",shmData->ticket, getTimeStamp());
    fprintf(fp, "creating new ticket %d | Time: %s \n",shmData->ticket, getTimeStamp());

    while(!(shmData->soldOut))
    {
      lockSem(semaphore);
      if(shmData->isTaken==true)
      {
        if(!(shmData->ticket==MAX_TICKETS))
        {
          shmData->ticket++;
          printf("creating new ticket %d | Time: %s \n",shmData->ticket, getTimeStamp());
          fprintf(fp, "creating new ticket %d | Time: %s \n",shmData->ticket, getTimeStamp());
          shmData->isTaken= false;
        }
        else if(shmData->ticket==MAX_TICKETS)
        {
          fprintf(fp, "tickets sold out. | Time: %s \n", getTimeStamp());;
          printf("tickets sold out | Time: %s \n", getTimeStamp());
          shmData->soldOut= true;
        }
      }
      unlockSem(semaphore);
    }

    closeSem(semaphore);
    destroySem(SEMNAME);

    fprintf(stderr, "Shared Memory Area destroyed | Time: %s \n", getTimeStamp());
    clearSHM(shmData);
    closeSHM(shmFd);
    destroySHM(SHNAME);
    fprintf(fp, "closing log file | Time: %s \n", getTimeStamp());
    fclose(fp);

    return EXIT_SUCCESS;
}
