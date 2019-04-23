// http://docs.oracle.com/cd/E19205-01/820-0619/gepji/index.html

/* din_philo.c */
#include <assert.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define PHILOS 5
#define DELAY 5000
#define FOOD 15

void *philosopher(void *id);
int food_on_table();
void init_queue();
void print_queue(int id);

// queue
int queue[PHILOS];
int q_begin, q_end;

// Mutexes
pthread_mutex_t chopstick[PHILOS];
pthread_mutex_t food_lock;
pthread_mutex_t q_lock;

// Threads
pthread_t philo[PHILOS];

int main(int argn, char **argv) {
  long i;

  // Inicia mutexes
  pthread_mutex_init(&food_lock, NULL);
  for (i = 0; i < PHILOS; i++)
    pthread_mutex_init(&chopstick[i], NULL);

  // filling queue with -1's
  init_queue();

  // Cria threads
  for (i = 0; i < PHILOS; i++)
    pthread_create(&philo[i], NULL, philosopher, (void *)i);
  // Join threads
  for (i = 0; i < PHILOS; i++)
    pthread_join(philo[i], NULL);

  return 0;
}

// Thread
void *philosopher(void *num) {
  long id = (long)num;
  long i, left_chopstick, right_chopstick, f;

  left_chopstick = id;
  right_chopstick = (id + 1) % PHILOS;
  f = 1;

  // acquiring a position in the queue
  pthread_mutex_lock(&q_lock);
  queue[q_end] = id;
  q_end++;
  print_queue(id);
  pthread_mutex_unlock(&q_lock);

  while (f) {
    // wait in the queue
    while (queue[q_begin] != id)
      ;
    pthread_mutex_lock(&chopstick[left_chopstick]);
    // printf ("Philosopher %ld: got left chopstick %ld\n", id, left_chopstick);
    pthread_mutex_lock(&chopstick[right_chopstick]);
    // printf ("Philosopher %ld: got right chopstick %ld\n", id,
    // right_chopstick);
    printf("Philosopher %ld: eating -- food %ld.\n", id, f);
    f = food_on_table();
    sleep(rand() % 5);

    pthread_mutex_lock(&q_lock);
    // giving place to the next in the queue
    if (q_begin < PHILOS)
      q_begin++;
    else
      q_begin = 0;
    pthread_mutex_unlock(&q_lock);

    pthread_mutex_unlock(&chopstick[left_chopstick]);
    pthread_mutex_unlock(&chopstick[right_chopstick]);
    int thinking_time = rand() % 10;
    printf("Philosopher %ld is done eating and is now thinking for %d "
           " secs.FOOD %ld\n",
           id, thinking_time, f);
    sleep(thinking_time);

    // putting this philosopher in queue's end
    pthread_mutex_lock(&q_lock);
    queue[q_end] = id;
    if (q_end < PHILOS)
      q_end++;
    else
      q_end = 0;
    print_queue(id);
    pthread_mutex_unlock(&q_lock);
  }
  return (NULL);
}

int food_on_table() {
  static int food = FOOD;
  int myfood;

  pthread_mutex_lock(&food_lock);
  if (food > 0) {
    food--;
  }

  myfood = food;
  pthread_mutex_unlock(&food_lock);

  return myfood;
}

void init_queue() {
  pthread_mutex_init(&q_lock, NULL);
  q_begin = 0;
  q_end = 0;

  for (int i = 0; i < PHILOS; i++) {
    queue[i] = -1;
  }
}

void print_queue(int thread_id) {
  printf("queue by thread %d: ", thread_id);
  for (int i = 0; i < PHILOS; i++) {
    printf("%d ", queue[i]);
  }
  putchar('\n');
  printf("begin: %d\nend: %d\n", q_begin, q_end);
}