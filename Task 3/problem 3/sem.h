#ifndef _SEM_H_
#define _SEM_H_

#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>

#define SEMNAME "/semserver" // shared semaphore

sem_t *createSem(char * semname,int value);
sem_t *loadSem(char * semname,int value);
void lockSem(sem_t *sem);
void unlockSem(sem_t *sem);
void closeSem(sem_t *sem);
void destroySem(char * semname);

#endif