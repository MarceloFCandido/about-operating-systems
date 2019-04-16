#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/resource.h>

#define NUM_THREADS 4
#define NUM_RUNS 1000
#define TRUE 1
#define FALSE 0

typedef int boolean;
typedef struct kit_t {
   long id;
   boolean *lock;
} KIT_t;

int x = 0;
bool waiting[NUM_THREADS];
int lock = 0;

boolean TSL(boolean *target) {
   boolean rv = *target;
   *target = true;
   return rv;
}

void * threadBody (void * kit){

   KIT_t *kit_casted = (KIT_t *) kit;
	long tid = kit_casted->id;
	int j,k,n = NUM_THREADS;
	bool key;
	
	do{
		/*sinaliza intencao do processo num. id de acessar a regiao critica */
		waiting[tid] = true;

		/*sinaliza recurso ocupado*/
		key = true;

		/*enquanto id esta esperando e o recurso ocupado, test and set*/
		while(waiting[tid] && key)
			key = TSL(&lock);

		/*espera acabou*/
		waiting[tid] = false;
		
		/*inicio da secao critica*/
		x++;
		/*fim da secao critica*/

		/*procura pelo proximo processo que esta esperando*/
		j = (tid + 1) % n;

		/*enquanto nao acha-lo, va pro proximo*/
		while((j!=tid) && !waiting[j])
			j = (j + 1) % n;

		/*se nao ha ninguem esperando, libera o recurso*/
		if(j == tid)
			lock = 0;
		else
			waiting[j] = false;

		x++;		
		
	} while (x < NUM_RUNS);
   	pthread_exit (NULL);
}

int main (int argc, char *argv[])
{
	/*aloca espaco para 5 threads*/
	pthread_t thread [NUM_THREADS];
	long i, status ;
   KIT_t kits[NUM_THREADS];

   boolean lock = FALSE;
	/*declara as 5 threads e executa em cada uma delas a funcao threadBody*/
	for (i = 0; i < NUM_THREADS; i++) {
      printf("%d",i);
      kits[i].id = i;
      kits[i].lock = &lock;
      status = pthread_create (&thread[i], NULL, threadBody, (void *) kits);
   }
	for (i = 0; i < NUM_THREADS; i++) {
		status = pthread_join (thread[i], NULL);
	}
	printf ("\n\nValor final de x: %02d\n\n", x);
   struct rusage ru;
   getrusage(RUSAGE_SELF, &ru);
   printf("\nInvoluntarias: %5ld \nVoluntarias: %5ld",ru.ru_nivcsw,ru.ru_nvcsw);
	printf ("\n\nValor final de x: %02d\n\n", x);		
	pthread_exit (NULL) ;
}