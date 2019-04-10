/*Este programa cria N threads que alteram o conteúdo
de uma variável global. Como não estão sendo utilizados
mecanismos de sincronização, o conteúdo da variável
se tornará inconsistente ao executar o programa.*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 4
#define NUM_RUNS 10
#define TRUE 1
#define FALSE 0

typedef int boolean;
typedef struct kit_t {
   long id;
   boolean *lock;
} KIT_t;

int x = 0;

boolean TSL(boolean *target) {
   boolean rv = *target;
   *target = TRUE;
   return rv;
}

void *threadBody (void *kit){

   KIT_t *kit_casted = (KIT_t *) kit;
   boolean *lock = kit_casted->lock;   
   long tid = kit_casted->id;
	
   int i;
	
   //Atualiza a variável global
	for (i = 0; i < NUM_RUNS; i++){
      while (TSL(lock))
         printf("Thread %ld: oi!", tid);
      x++;
      *lock = FALSE;
   }

   pthread_exit (NULL);
}

int main (int argc, char *argv[]) {
   pthread_t thread [NUM_THREADS];
   long i, status;
   KIT_t kits[NUM_THREADS];

   boolean lock = FALSE;

   for (i = 0; i < NUM_THREADS; i++) {
      kits[i].id = i;
      kits[i].lock = &lock;
      status = pthread_create (&thread[i], NULL, threadBody, (void *) kits);
   }
	for (i = 0; i < NUM_THREADS; i++) {
		status = pthread_join (thread[i], NULL);
	}

	printf ("\n\nValor final de x: %02d\n\n", x);	
   pthread_exit (NULL) ;
}
