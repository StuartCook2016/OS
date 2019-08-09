//complie with  gcc -Wall -g problem1.c -pthread -o problem1


#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <sys/timeb.h>

#define NUM_POINTS 5
#define NUM_COMMANDS 2

#define N_THREADS 3
#define BUFFER_SIZE 200
#define N_DATA 10000
#define WORKLOAD1 1000
#define WORKLOAD2 1000
#define WORKLOAD3 1000
//#define OUTPUT

pthread_mutex_t mutex1,mutex2 = PTHREAD_MUTEX_INITIALIZER;


double maxtime0 = 0;
double mintime0 = 10000;
double avragetime0 = 0;

double maxtime1 = 0;
double mintime1 = 10000;
double avragetime1 = 0;

double maxtime2 = 0;
double mintime2 = 10000;
double avragetime2 = 0;
/*******************************************************************************
 **
 ** Here, the buffer implementation:
 **
 ******************************************************************************/



 typedef struct buffer buffer_t;

 struct buffer {
   volatile int head;
   volatile int tail;
   int size;
   volatile int *elems;
 };

 buffer_t *createBuffer( int size)
 {
   buffer_t *buf;

   buf = (buffer_t *)malloc( sizeof(buffer_t));
   buf->head = 0;
   buf->tail = 0;
   buf->size = size+1;
   buf->elems = (int *)malloc( (size+1)*sizeof(int));

   return(buf);
 }

int pop( buffer_t* buf, int *data)
{
  pthread_mutex_lock(&mutex1);
  int res;

  if(buf->head == buf->tail) {
      res = 0;
  } else {
    *data = buf->elems[buf->head];
    buf->head = (buf->head+1) % buf->size;
    res = 1;
  }
  pthread_mutex_unlock(&mutex1);
  return(res);
}


int push( buffer_t* buf, int data)
{
  pthread_mutex_lock(&mutex2);
  int nextTail;
  int res;

  nextTail = (buf->tail + 1) % buf->size;
  if(nextTail != buf->head)   {
    buf->elems[buf->tail] = data;
    buf->tail = nextTail;
    res = 1;
  } else {
    res = 0;
  }
  pthread_mutex_unlock(&mutex2);
  return(res);
}

/*******************************************************************************
 **
 ** Now, the thread functions for the pipelining:
 **
 ******************************************************************************/

typedef struct threadArgs threadArgs_t;

struct threadArgs {
  int tid;
  buffer_t *in_buf;
  buffer_t *out_buf;
  int workload;
};

int workUnit( int data)
{
  if( data < 0)
    data++;

  return(data);
}

int process( int tid, int data, int  workload)
{
  struct timespec begin0, end0, begin1, end1, begin2, end2;

  if (tid == 0)
    clock_gettime(CLOCK_MONOTONIC_RAW, &begin0);

  if (tid == 1)
    clock_gettime(CLOCK_MONOTONIC_RAW, &begin1);

  if (tid == 2)
    clock_gettime(CLOCK_MONOTONIC_RAW, &begin2);

#ifdef OUTPUT
  //  printf( "[%d] processing item %d!\n", tid, data);


#endif

  for(int i = 0; i < workload; i++)
    data = workUnit(data);

#ifdef OUTPUT

  printf( "[%d] item %d done!\n", tid, data);


if ((data%2499) == 0){

#endif






if (tid == 0){
clock_gettime(CLOCK_MONOTONIC_RAW, &end0);
  double temp0 = (end0.tv_nsec - begin0.tv_nsec) / 1000000000.0 +
           (end0.tv_sec  - begin0.tv_sec);

    if (temp0 < mintime0){
      mintime0 = temp0;
    }
    if (temp0 > maxtime0){
      maxtime0 = temp0;
    }

    avragetime0 += temp0;
}

if (tid == 1){
clock_gettime(CLOCK_MONOTONIC_RAW, &end1);
  double temp1 = (end1.tv_nsec - begin1.tv_nsec) / 1000000000.0 +
           (end1.tv_sec  - begin1.tv_sec);

    if (temp1 < mintime1){
      mintime1 = temp1;
    }
    if (temp1 > maxtime1){
      maxtime1 = temp1;
    }

    avragetime1 += temp1;
}
if (tid == 2){
clock_gettime(CLOCK_MONOTONIC_RAW, &end2);
  double temp2 = (end2.tv_nsec - begin2.tv_nsec) / 1000000000.0 +
           (end2.tv_sec  - begin2.tv_sec);

    if (temp2 < mintime2){
      mintime2 = temp2;
    }
    if (temp2 > maxtime2){
      maxtime2 = temp2;
    }

    avragetime2 += temp2;
}

  return( data);
}

void * pipeline( void *arg)
{
  int data;
  int workload;
  buffer_t *in;
  buffer_t *out;
  int tid;

  in = ((threadArgs_t *)arg)->in_buf;
  out = ((threadArgs_t *)arg)->out_buf;
  tid = ((threadArgs_t *)arg)->tid;
  workload = ((threadArgs_t *)arg)->workload;

//  Here, your input is required :-)
  while(1)
  {
    while(!pop(in,&data))
      pthread_yield();
    process(tid,data,workload);
    while(!push(out,data))
      pthread_yield();
  }
}

/*******************************************************************************
 **
 ** main
 **
 ******************************************************************************/




int main()
{
int poppin = 0;
  //struct timeb tmb;
    //ftime(&tmb);
    //printf("tmb.time     = %ld (seconds)\n", tmb.time);
    //printf("tmb.millitm  = %d (mlliseconds)\n", tmb.millitm);

  struct timespec begin, end;
  clock_gettime(CLOCK_MONOTONIC_RAW, &begin);

  int i, suc;
  int data;

  threadArgs_t args[N_THREADS];
  pthread_t threads[N_THREADS];
  buffer_t *in, *inter1, *inter2, *out;

  in = createBuffer( N_DATA+1);
  inter1 = createBuffer( BUFFER_SIZE);
  inter2 = createBuffer( BUFFER_SIZE);
  out = createBuffer( N_DATA+1);

  /**
   *
   * First, we start our threads:
   */

  args[0].tid=0;
  args[0].in_buf=in;
  args[0].out_buf=inter1;
  args[0].workload=WORKLOAD1;

  args[1].tid=1;
  args[1].in_buf=inter1;
  args[1].out_buf=inter2;
  args[1].workload=WORKLOAD2;

  args[2].tid=2;
  args[2].in_buf=inter2;
  args[2].out_buf=out;
  args[2].workload=WORKLOAD3;

  for(i = 0; i < N_THREADS; i++)
  {
    suc = pthread_create(&threads[i],NULL,pipeline,&args[i]);
    if(suc != 0 ){  /* status !=0 indicates an error in the thread creation */
      perror("pthread_create");
      exit(1);  /* exit with error */
    }
  }

  /**
   * Then, we fill the input buffer:
   */


  for(i = 0; i < N_DATA; i++)
  {
    data = i;
    while(!push(in,data)) /* push numbers in sequence */
    pthread_yield();


  }

  /**
   * Finally, we observe the output in the buffer "out":
   */

  for(i = 0; i < N_DATA; i++)
  {
    while(!pop(out,&data)) /* pop numbers from queue */
      pthread_yield();
      poppin++;


  //  clock_gettime(CLOCK_MONOTONIC_RAW, &end1);


    //    printf ("Total time = %f seconds\n",
    //            (end1.tv_nsec - begin.tv_nsec) / 1000000000.0 +
    //            (end1.tv_sec  - begin.tv_sec));
    //  }

  }

  clock_gettime(CLOCK_MONOTONIC_RAW, &end);

  double timer = (end.tv_nsec - begin.tv_nsec) / 1000000000.0 +
           (end.tv_sec  - begin.tv_sec);

  double throughput = poppin / timer;


   printf ("Total time = %f seconds\n",
           (end.tv_nsec - begin.tv_nsec) / 1000000000.0 +
           (end.tv_sec  - begin.tv_sec));

printf("poppin %d\n\n", poppin);
  printf("Throughput %f\n\n", throughput);

  printf("Min for 0 = %f\n", mintime0);
  printf("Max for 0 = %f\n", maxtime0);
  printf("Average for 0 = %f\n\n", (avragetime0 / N_DATA));

  printf("Min for 1 = %f\n", mintime1);
  printf("Max for 1 = %f\n", maxtime1);
  printf("Average for 1 = %f\n\n", (avragetime1 / N_DATA));

  printf("Min for 2 = %f\n", mintime2);
  printf("Max for 2 = %f\n", maxtime2);
  printf("Average for 2 = %f\n\n", (avragetime2 / N_DATA));

  printf("Done!\n");

  return(0);
}
