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

int main(int argc, char const *argv[]) {

  int shm_fd;
  int *ptr;
  uintptr_t vaddr, paddr = 0;

  // getting reader's id
  int id = atoi(argv[1]);
  int others_pid = id ? 0 : 1;

  // Opening shared memory created by sender
  shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);

  // Mapping the shared memory allocated by sender to the process's address
  // space
  ptr =
      mmap(0, 7 * sizeof(long), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
  if (ptr == MAP_FAILED) {
    printf("Process %d: Map failed\n", getpid());
    return -1;
  }

  ptr[id + 1] = getpid();

  // looking for the logical address of the shared variable
  printf("Process %d - logical address: %p\n", getpid(), ptr);
  ptr[id + 3] = (uintptr_t)ptr;

  while (ptr[others_pid + 1] == 0 && ptr[others_pid + 3] == 0)
    ;

  printf("Process %d sees process %d\n", getpid(), ptr[others_pid + 1]);

  if (virt_to_phys_user(&paddr, (uintmax_t)ptr[others_pid + 1],
                        (uintptr_t)ptr[others_pid + 3])) {
    fprintf(stderr, "error: virt_to_phys_user\n");
    ptr[id + 5] = 1;
    return 1;
  };

  ptr[id + 5] = 1;
  while (ptr[others_pid + 5] == 0)
    ;

  // exibiting physical address found
  printf("0x%ldx\n", paddr);

  return 0;
}
