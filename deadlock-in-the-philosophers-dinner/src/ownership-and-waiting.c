//http://docs.oracle.com/cd/E19205-01/820-0619/gepji/index.html


/* din_philo.c */    
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <time.h>

#define PHILOS 5
#define DELAY 5000
#define FOOD 15000

void *philosopher (void *id);
int food_on_table ();

//Mutexes
pthread_mutex_t chopstick[PHILOS];
pthread_mutex_t food_lock;

//Threads
pthread_t philo[PHILOS];

int lock = 0;
int palito_ocupado[PHILOS];

int main (int argn, char **argv)
{
	long i;
	srand(time(0));
	// starting mutexes
	pthread_mutex_init (&food_lock, NULL);
	for (i = 0; i < PHILOS; i++){
		pthread_mutex_init (&chopstick[i], NULL);
		palito_ocupado[i]=0;
	}
	// creating threads
	for (i = 0; i < PHILOS; i++)
		pthread_create (&philo[i], NULL, philosopher, (void *)i);
	// Joining threads	
	for (i = 0; i < PHILOS; i++)
		pthread_join (philo[i], NULL);
	return 0;
}

// Thread
void* philosopher(void *num)
{
	long id;
	int i, left_chopstick, right_chopstick, f;
	id = (long)num;
	left_chopstick = id;
	right_chopstick = (id + 1)%PHILOS;

	if (id == 0)
		usleep(rand()%2000);

	do {

		while(__sync_lock_test_and_set(&lock, 1));
            // When a stick has been catched, the another stick can't be catched
			while(palito_ocupado[left_chopstick] || palito_ocupado[right_chopstick]);
            // Catch a Stick[left]
			pthread_mutex_lock(&chopstick[left_chopstick]);
			printf ("Philosopher %ld: got chopstick %d\n", id,  left_chopstick); palito_ocupado[left_chopstick] = 1;
            // catch a Stick[Right]
			pthread_mutex_lock(&chopstick[right_chopstick]);
			printf ("Philosopher %ld: got chopstick %d\n", id,  right_chopstick); palito_ocupado[right_chopstick] = 1;
		
		lock = 0;
		
		f = food_on_table ();

		printf ("Philosopher %ld: eating -- food %d.\n", id, f);
        // Eat for 200 ms 
		usleep(rand()%200); 
		printf ("Philosopher %ld has finished.\n", id);

		pthread_mutex_unlock(&chopstick[left_chopstick]);
		printf("Philosopher %ld: released chopstick %d\n", id,  left_chopstick); palito_ocupado[left_chopstick] = 0;
		pthread_mutex_unlock(&chopstick[right_chopstick]);
		printf("Philosopher %ld: released chopstick %d\n", id,  right_chopstick); palito_ocupado[right_chopstick] = 0;
        
        //Think about your life
		usleep (rand()%200); 

	} while(f > 0);

	return (NULL);
}

int food_on_table ()
{
    static int food = FOOD;
    int myfood;

    pthread_mutex_lock (&food_lock);
    if (food > 0) {
        food--;
    }
    myfood = food;
    pthread_mutex_unlock (&food_lock);
    return myfood;
}
