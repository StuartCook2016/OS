#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

int var = 0;
int var1 = 0;

int main()
{

	pid_t pid;
	int status;

  pid = fork();


	switch(pid)
	{
		case -1:
			printf("fork failed");
			break;
		case 0:
      printf( "%d \n", var);
			printf("I am the child process %d \n" , getpid());

      while (var == 0){

      }

      printf("The child is done %d \n" , getpid());
      break;



		default:

			printf("I am the parent process %d \n" , getpid());


      printf( "%d \n" , var);
      printf("The parent is done %d \n" , getpid());
      var = 1;
            printf( "%d \n" , var);
      waitpid(-1,&status, 0);

	}




	return 0;
}
