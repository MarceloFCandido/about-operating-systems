/*Este programa cria N threads que alteram o conteúdo
de uma variável global. Como não estão sendo utilizados
mecanismos de sincronização, o conteúdo da variável
se tornará inconsistente ao executar o programa.*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>

#define NUM_THREADS 4
#define NUM_RUNS 5
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
   boolean *lock = kit_casted->lock,key;   
   long tid = kit_casted->id;
	
   int i,j;
	
   //Atualiza a variável global
	for (i = 0; i < NUM_RUNS; i++){
      *(kit_casted[i].lock) = TRUE; //Indica que o processo i está esperando
      key = TRUE; //Indica se o recurso está ocupado ou não
      while (*(kit_casted[i].lock) && key){
         key = TSL(lock); //Sai do “while” quando recurso livre

      }
      *(kit_casted[i].lock)= TRUE;
      x++;
      /* Executa a seção crítica */
      /*Procura na lista pelo o próximo processo que está esperando*/
      j = (i + 1) % NUM_RUNS; //Inicia a busca no próximo
      while ((j != i) && !kit_casted[j].lock) //Enquanto não encontrar
         j = (j + 1) % NUM_RUNS; //Passa para o próximo
      if (j == i) //Se não há processos esperando
         *lock = FALSE; //Libera o recurso
      else
         *(kit_casted[j].lock) = FALSE; //Libera o processo “j” do “while” na linha 4.
 /*executa o restante da seção */ 
         
      
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
   struct rusage ru;
   getrusage(RUSAGE_SELF, &ru);
   printf("\nInvoluntarias: %5ld \nVoluntarias: %5ld",ru.ru_nivcsw,ru.ru_nvcsw);
	printf ("\n\nValor final de x: %02d\n\n", x);	
   pthread_exit (NULL) ;
}
