#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <limits.h>
#include <semaphore.h>
#include <time.h>
#include "check.h"
#include <syscall.h>

/* The mutex lock for the writer*/
pthread_mutex_t r_mutex;
/* mutual exclusion to update a variable (write_count --> current number
of readers*/
pthread_mutex_t mutex;

/* current number of readers */
int write_count = 0;
/*shared data*/
int x = 1;

pthread_t tid;       //Thread ID
pthread_attr_t attr; //Set of thread attributes

void *writer(void *param);
void *reader(void *param);

void *reader(void *param)
{
   while (1)
   {
      pthread_mutex_lock(&r_mutex);
      /****LEITURA DO DADO****/
      printf("\n(%07d) -- read: %05d", syscall(SYS_gettid), x);
      pthread_mutex_unlock(&r_mutex);
      sleep(1);
   }
}

void *writer(void *param)
{
   sleep(1);
   while (1)
   {
      /***Atualiza: write_count***/
      pthread_mutex_lock(&mutex);
      write_count++;
      /***O primeiro escritor bloqueia os leitores***/
      if (write_count == 1)
         pthread_mutex_lock(&r_mutex);
      pthread_mutex_unlock(&mutex);
      x++;
      printf("\n(%07d) -- wrote: %05d", syscall(SYS_gettid), x);
      sleep(1);
      /***Atualiza: write_count***/
      pthread_mutex_lock(&mutex);
      write_count--;
      //O último leitor libera os escritores.
      if (write_count == 0)
         pthread_mutex_unlock(&r_mutex);
      pthread_mutex_unlock(&mutex);
      //sleep(rand()%10+1);
      sleep(rand() % 3 + 1);

   }
}

int main(int argc, char *argv[])
{
   srand(time(0));
   /* Verify the correct number of arguments were passed in */
   if (argc != 4)
   {
      fprintf(stderr, "USAGE:./main.out <INT> <INT> <INT>\n");
      exit(1);
   }

   int mainSleepTime = atoi(argv[1]); /* Time in seconds for main to sleep */
   int numWriter = atoi(argv[2]);     /* Number of writers*/
   int numReader = atoi(argv[3]);     /* Number of readers*/

   /* Initialize the app */
   pthread_mutex_init(&r_mutex, NULL);
   pthread_mutex_init(&mutex, NULL);
   write_count = 0;

   pthread_t *thread = malloc((numWriter + numReader) * sizeof(pthread_t));
   /* Create the writer threads */
   int i;
   for (i = 0; i < numWriter; i++)
   {
      /* Create the thread */
      thread[i] = pthread_create(&tid, &attr, writer, NULL);
   }

   /* Create the reader threads */
   for (i = 0; i < numReader; i++)
   {
      /* Create the thread */
      thread[numReader + i] = pthread_create(&tid, &attr, reader, NULL);
   }

   /* Sleep for the specified amount of time in milliseconds */
   sleep(mainSleepTime);

   /* Exit the program */
   printf("\nExit the program\n");
   exit(0);
}
