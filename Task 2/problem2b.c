#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>
#include <pthread.h>

void *printFUNC(){
  printf("Child process\n");
printf("Chlid process is done\n");
}


int main()
{
    pthread_t tid;
    pthread_create(&tid, NULL, printFUNC, NULL);
    printf("Parent Process \n");
    pthread_join(tid, NULL);

    printf("Parent process is done \n");

  exit(0);


}
