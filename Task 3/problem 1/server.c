#include <stdio.h>
#include <stdlib.h>

#include "shm.h"

int main()
{
      system("clear");

    FILE * fp;
    int shmFd;
    SHMstruct initData = { 1, false, false};
    SHMstruct * shmData;

    fp = fopen("/tmp/server.log", "a");
    if(fp == NULL) {
        perror("fopen");
        return EXIT_FAILURE;
    }
    fprintf(fp,"Log file opened \n | Time: %s", getTimeStamp());

    fprintf(stderr, "Shared Memory Area created | Time: %s \n", getTimeStamp());
    shmFd = createSHM(SHNAME);
    shmData = initSHM( shmFd, &initData );
    printf("add: %p \n", shmData);


    fprintf(fp, "Shared Memory Area created | Time: %s \n", getTimeStamp());


    fclose(fp);
    fp = fopen("/tmp/server.log", "a");

    printf(" creating new ticket %d| Time:  %s \n", shmData->ticket, getTimeStamp());

    fprintf(fp, "creating new ticket %d | Time: %s   \n", shmData->ticket, getTimeStamp());

    while(!(shmData->soldOut))
    {
      if(shmData->isTaken==true){

        if(!(shmData->ticket==MAX_TICKETS)){
          shmData->ticket++;
          printf("creating new ticket %d | Time: %s \n", shmData->ticket, getTimeStamp());
          fprintf(fp, "creating new ticket %d | Time: %s \n", shmData->ticket, getTimeStamp());
          shmData->isTaken= false;
        }
        else if(shmData->ticket==MAX_TICKETS){

          fprintf(fp, "tickets sold out. %s | Time \n ", getTimeStamp());
          printf("tickets sold out. %s | Time \n ", getTimeStamp());
          shmData->soldOut= true;
          }
      }
      }


    fprintf(stderr, "Shared Memory Area destroyed %s \n", getTimeStamp());
    clearSHM(shmData);
    closeSHM(shmFd);
    destroySHM(SHNAME);
    fprintf(fp, "closing log file %s \n", getTimeStamp());
    fclose(fp);

    return EXIT_SUCCESS;
}
