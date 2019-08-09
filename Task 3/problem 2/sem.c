#include <stdbool.h>
#include <time.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "sem.h"

sem_t *createSem(char * semname,int value)
{
  sem_t *sem = sem_open(semname, O_CREAT | O_RDWR, S_IRWXU|S_IRWXG, value); /* create the Semaphore */
  if (sem == SEM_FAILED) {
    perror("createSem error");
    exit(EXIT_FAILURE);
  }
  return sem;

}

sem_t *loadSem(char * semname,int value){

  sem_t *sem = sem_open(semname, O_RDWR); /* load the Semaphore */
  if (sem == SEM_FAILED) {
    perror("loadSem: sem_open");
    exit(EXIT_FAILURE);
  }
  return sem;


}

void lockSem(sem_t *sem)
{
  int result = sem_wait(sem);

    if(result!=0){
      perror("lockSem error");
      exit(EXIT_FAILURE);
    }

}

void unlockSem(sem_t *sem) {
  int result = sem_post(sem);

    if(result!=0){
      perror("lockSem error");
      exit(EXIT_FAILURE);
    }
}

void closeSem(sem_t *sem){

	int check = sem_close(sem);
	if(check == -1){
		perror("Error closing semaphore");
	}
}

void destroySem(char * semname){

	int check = sem_unlink(semname);
	if(check == -1){
		perror("Error destroying semaphore");
	}
}