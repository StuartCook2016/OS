#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>
#include <pthread.h>

  int state1=1;
  int state2=1;
  int state3=1;

  pthread_mutex_t mutex1,mutex2,mutex3 = PTHREAD_MUTEX_INITIALIZER;
  pthread_cond_t cond1, cond2, cond3 = PTHREAD_COND_INITIALIZER;


void *printFUNC(){
  //child thread

  //waiting on parent thread
  pthread_mutex_lock(&mutex1);
  while (state1 == 1){
    pthread_cond_wait(&cond1, &mutex1);
  }
  pthread_mutex_unlock(&mutex1);
  printf("Child thread started \n");

    //signling the parent
    pthread_mutex_lock(&mutex2);
    state2 = 0;
    pthread_cond_signal(&cond2);
    pthread_mutex_unlock(&mutex2);

    //waiting on the parent
      pthread_mutex_lock(&mutex3);
      while (state3 == 1){
        pthread_cond_wait(&cond3, &mutex3);
      }
      pthread_mutex_unlock(&mutex3);
      printf("Child thread is done \n");

    return(0);


}


int main()
{
    //parent thread

    pthread_t tid;
    pthread_create(&tid, NULL, printFUNC, NULL);
    printf("Parent thread started \n");

    //unlocking and signaling the child
    pthread_mutex_lock(&mutex1);
    state1 = 0;
    pthread_cond_signal(&cond1);
    pthread_mutex_unlock(&mutex1);

    // waiting on the child
    pthread_mutex_lock(&mutex2);
    while (state2 == 1){
      pthread_cond_wait(&cond2, &mutex2);
    }
    pthread_mutex_unlock(&mutex2);
    printf("Parent thread is done \n");


    //signling the child
    pthread_mutex_lock(&mutex3);
    state3 = 0;
    pthread_cond_signal(&cond3);
    pthread_mutex_unlock(&mutex3);

    //waiting on child to ensure its totally finished
    pthread_join(tid, NULL);



}
