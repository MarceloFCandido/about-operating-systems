#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define SHM_NAME "shared_memory"
#define TIMES     7

int main(int argc, char const *argv[]) {

  int shm_fd;
  long *ptr;

  // Creating shared memory for communication between processes
  shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);

  // Defining shared memory space
  ftruncate(shm_fd, TIMES * sizeof(long));

  // Mapping the shared memory allocated to the process's address space
  ptr = mmap(0, TIMES * sizeof(long), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
  if (ptr == MAP_FAILED) {
    printf("Map failed\n");
    return -1;
  }

  printf("Creator's pid: %d\n", getpid());

  // putting an arbitrary number at the shared memory
  ptr[0] = 10;    // data
  ptr[1] = 0;     // processor's pid
  ptr[2] = 0;
  ptr[3] = 0;     // virtual address of the variable for processor 'id'
  ptr[4] = 0;
  ptr[5] = 0;     // work terminated by id
  ptr[6] = 0;

  // codition for the termination of the program
  while (ptr[5] == 0 && ptr[6] == 0)
    ;

  // reset memory
  if (shm_unlink(SHM_NAME) == -1) {
    printf("Error removing %s\n", SHM_NAME);
    exit(-1);
  }

  return 0;
}
