#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>

#include "shm.h"
#include "sem.h"

int main()
{
    FILE * fp;
    int shmFd, shmFd2, shmFd3;
    SHMstruct * shmData;
    SHMstruct2 * shmData2;
    SHMstruct3 * shmData3;
    sem_t *semaphore;


    while(access("/dev/shm/shmserver", F_OK) == -1)
    {
      printf("Waiting for server... \n");
      sleep(MAX_SLEEP);
      system("clear");
    }
    system("clear");
    printf("Server found. \n");

    fp = fopen("/tmp/client.log", "a");
    if(fp == NULL)
    {
        perror("fopen, Error opening file");
        return EXIT_FAILURE;
    }
    fprintf(fp,"Log file opened \n");

    shmFd = loadSHM(SHNAME);    
    shmData = accessSHM(shmFd);
    shmData2 = accessSHM(shmFd);
    shmData3 = accessSHM(shmFd);

    semaphore = loadSem(SEMNAME, 1);

    while(!(shmData->soldOut) && !(shmData2->soldOut) && !(shmData2->soldOut))
    {
        lockSem(semaphore);
        if(shmData->isTaken == false)
        {
            if(shmData->ticket != MAX_TICKETS){
                fprintf(fp,"Ticket not taken, sleeping \n");
                sleep(MAX_SLEEP);
                printf("Taking ticket %d in channel 1\n",shmData->ticket);
                fprintf(fp,"taking ticket %d in channel 1\n",shmData->ticket);
                shmData->isTaken = true;
            }
            else if(shmData->ticket==MAX_TICKETS){
                sleep(MAX_SLEEP);
                shmData->isTaken = true;
                printf("Taking ticket %d in channel 1\n",shmData->ticket);
                fprintf(fp,"taking ticket %d in channel 1\n",shmData->ticket);
                printf("No more tickets in channel 1\n");
                fprintf(fp,"taking ticket in channel 1\n");
            printf("No more tickets in channel 1\n" );
            }
        }

        if(shmData2->isTaken == false)
        {
            if(shmData2->ticket != MAX_TICKETS){
                fprintf(fp,"Ticket not taken, sleeping \n");
                sleep(MAX_SLEEP);
                printf("Taking ticket %d in channel 2\n",shmData2->ticket);
                fprintf(fp,"taking ticket %d in channel 2\n",shmData2->ticket);
                shmData2->isTaken = true;
            }
            else if(shmData2->ticket==MAX_TICKETS){
                sleep(MAX_SLEEP);
                shmData2->isTaken = true;
                printf("Taking ticket %d in channel 2\n",shmData2->ticket);
                fprintf(fp,"taking ticket %d in channel 2\n",shmData2->ticket);
                printf("No more tickets in channel 2\n");
                fprintf(fp,"taking ticket in channel 2\n");
            }
        }

    unlockSem(semaphore);
    sleep(MAX_SLEEP);
  }

    fprintf(fp,"tickets Sold out, Closing log file and clearing memory.\n");

    closeSem(semaphore);

    clearSHM(shmData);
    closeSHM(shmFd);
    fclose(fp);

return EXIT_SUCCESS;
}