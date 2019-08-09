#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>
int main()
{
    int status;
    int pid;
    pid=fork();



    if(pid<0)
    {
        printf("\n Error ");
        exit(1);
    }
    else if(pid==0)
      {

        printf("\n Hello I am the child process. ");
        printf("\n the child process is done.");
        
        exit(0);

    }
        printf("\n Hello I am the parent process. ");
        fflush(stdout);
        waitpid(-1,&status, 0);
        printf("\n the parent process is done.");
        printf("\n process id =  %d \n",getpid());
        exit(1);


}
