#include "common.h"
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
#define TIMES 7

void print_sh_mem(int id, long *ptr);

int main(int argc, char const *argv[]) {

  int shm_fd;
  long *ptr;
  uintptr_t vaddr, paddr = 0;

  // getting my own id
  int id = atoi(argv[1]);
  int others_pid = id ? 0 : 1;

  // Opening shared memory created by sender
  shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);

  // Mapping the shared memory allocated by sender to the process's address
  // space
  ptr = mmap(0, TIMES * sizeof(long), PROT_READ | PROT_WRITE, MAP_SHARED,
             shm_fd, 0);
  if (ptr == MAP_FAILED) {
    printf("Process %d: Map failed\n", getpid());
    return -1;
  }

  // putting my own pid on the shared memory
  ptr[id + 1] = (long)getpid();

  // looking for the logical address of my shared variable
  printf("Process %d - logical address: %p\n", getpid(), ptr);
  ptr[id + 3] = /*(uintptr_t)*/ (long)ptr;

  while (ptr[others_pid + 1] == 0 && ptr[others_pid + 3] == 0)
    ; // the other process put its pid and logical address in the right place

  printf("Process %d sees process %ld\n", getpid(), ptr[others_pid + 1]);

  print_sh_mem(id, ptr);
  if (virt_to_phys_user(&paddr, ptr[id + 1], (uintptr_t)ptr[id + 3])) {
    fprintf(stderr, "error: virt_to_phys_user\n");
    ptr[id + 5] = 1;
    return 1;
  }

  ptr[id + 5] = 1;
  while (ptr[others_pid + 5] == 0)
    ;

  // exibiting physical address found
  printf("id: %d : 0x%jx\n", id, paddr);

  return 0;
}

void print_sh_mem(int id, long *ptr) {
  printf("id: %d - data: %ld\n", id, ptr[0]);
  printf("id: %d - la1 : %p\n", id, (int *)ptr[3]);
  printf("id: %d - la2 : %p\n", id, (int *)ptr[4]);
}