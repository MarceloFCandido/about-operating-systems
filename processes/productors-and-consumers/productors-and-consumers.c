#include <pthread.h>
#include <stdio.h>
#include <time.h>

// global variables
int x;
int nWriters;
int nReaders;
pthread_mutexattr_t rw_lock;
pthread_mutexattr_t update_lock;
typedef struct kit {
  int id;
} KIT_t;

int main(int argc, char *argv[]) {

  srand(time(0));
  // program test
  if (argc != 4) {
    printf(
        "Wrong number of arguments\nUsage: make run mst=int nw=int nr=int\n");
    exit(1);
  }

  int mainSleep;

  mainSleep = atoi(argv[1]);
  nReaders = atoi(argv[2]);
  nWriters = atoi(argv[3]);

  pthread_t RThreads[nReaders], WThreads[nWriters];
  pthread_attr_t attr;
  KIT_t RKits[nReaders], WKits[nWriters];

  // initing pthread variables
  pthread_mutex_init(&rw_lock, NULL);
  pthread_mutex_init(&update_lock, NULL);
  pthread_attr_init(&attr);

  // creating reader threads
  for (int i = 0; i < nReaders; i++) {
    RKits[i].id = i;
    if (pthread_create(&RThreads[i], &attr, reader, (void *)&RKits[i])) {
      printf("Failed to create reader thread %d.", i);
      exit(1);
    }
  }

  for (int i = 0; i < nWriters; i++) {
    if (pthread_create(&WThreads[i], &attr, writer, (void *)&WKits[i])) {
      printf("Failed to create writer thread %d.", i);
      exit(1);
    }
  }

  sleep(mainSleep);

  return 0;
}
