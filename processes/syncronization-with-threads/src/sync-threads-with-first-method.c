/*Este programa cria N threads que alteram o conteúdo
de uma variável global. Como não estão sendo utilizados
mecanismos de sincronização, o conteúdo da variável
se tornará inconsistente ao executar o programa.*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdbool.h>
#include <sys/resource.h>

#define NUM_THREADS 4
#define NUM_RUNS 10
#define TRUE 1
#define FALSE 0

typedef int boolean;
typedef struct kit_t {
   long id;
   bool *lock;
} KIT_t;

int x = 0;

boolean TSL(bool *target) {
   boolean rv = *target;
   *target = true;
   return rv;
}

void *threadBody (void *kit){

   KIT_t *kit_casted = (KIT_t *) kit;
   bool *lock = kit_casted->lock;
   long tid = kit_casted->id;

   int i;

   //Atualiza a variável global
	for (i = 0; i < NUM_RUNS; i++){
        while (TSL(lock))
        printf("Thread %ld: oi!", tid);
        x++;
        *lock = false;
    }

   pthread_exit (NULL);
}

int main (int argc, char *argv[]) {
    pthread_t thread [NUM_THREADS];
    long i, status;
    KIT_t kits[NUM_THREADS];

    bool lock = false;

    for (i = 0; i < NUM_THREADS; i++) {
        kits[i].id = i;
        kits[i].lock = &lock;
        status = pthread_create (&thread[i], NULL, threadBody, (void *) kits);
    }
    for (i = 0; i < NUM_THREADS; i++) {
        status = pthread_join (thread[i], NULL);
    }
    
    struct rusage ru;
    getrusage(RUSAGE_SELF, &ru);
    printf("\nInvoluntarias: %5ld \nVoluntarias: %5ld",ru.ru_nivcsw,ru.ru_nvcsw);
    printf ("\n\nValor final de x: %02d\n\n", x);
    pthread_exit (NULL) ;
}
