
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>


#include "shm.h"

int main()
{


    FILE * fp;
    int shmFd;
    SHMstruct * shmData;


    while(access("/dev/shm/shmserver", F_OK) == -1)
    {
      printf("Waiting for server... | Time: %s \n", getTimeStamp());
      sleep(MAX_SLEEP);
      system("clear");
    }
    system("clear");
    printf("Server found. | Time: %s \n", getTimeStamp());



    fp = fopen("/tmp/client.log", "a");
    if(fp == NULL) {
        perror("fopen, Error opening file");
        return EXIT_FAILURE;
    }
    fprintf(fp,"Log file opened | Time: %s \n", getTimeStamp());




      shmFd = loadSHM(SHNAME);
    shmData = accessSHM(shmFd);
    printf("add: %p \n", shmData);


    while(!(shmData->soldOut))
    {
      if(shmData->isTaken==false && shmData->ticket != MAX_TICKETS){
        fprintf(fp,"Ticket not taken, sleeping  | Time: %s \n", getTimeStamp());
        sleep(MAX_SLEEP);
        printf("Taking ticket %d | Time: %s \n",shmData->ticket, getTimeStamp());
        fprintf(fp,"taking ticket %d | Time: %s \n",shmData->ticket, getTimeStamp());
                shmData->isTaken = true;
      }
      else if(shmData->ticket==MAX_TICKETS){
        sleep(MAX_SLEEP);
                shmData->isTaken = true;
        printf("Taking ticket %d  | Time: %s \n",shmData->ticket, getTimeStamp());
        fprintf(fp,"taking ticket %d  | Time: %s \n",shmData->ticket, getTimeStamp());

        printf("No more tickets |Time: %s \n", getTimeStamp());
        fprintf(fp,"No more tickets |Time: %s \n", getTimeStamp());
    }
  }

    fprintf(fp,"tickets Sold out, Closing log file and clearing memory. |Time: %s \n", getTimeStamp());

    clearSHM(shmData);
    closeSHM(shmFd);
    fclose(fp);

    return EXIT_SUCCESS;
}
