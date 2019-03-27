#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>

#define SHM_SIZE 1024
#define SHM_NAME "shared_memory"

int main(int argc, char const *argv[]) {

    srand(time(0));

    int shm_fd;
    char *ptr, *start;

    int n_proc; // for receiving the number of receivers
    n_proc = atoi(argv[1]);

    // Creating shared memory for communication between processes
    shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);

    // Defining shared memory space
    ftruncate(shm_fd, SHM_SIZE + n_proc * sizeof(int));

    // Mapping the shared memory allocated to the process's address space
    ptr = mmap(0, SHM_SIZE + n_proc * sizeof(int), PROT_READ | PROT_WRITE,
        MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        printf("Map failed\n");
        return -1;
    }

    // Writing randomly in the shared memory
    start = ptr;
    for (int i = 0; i < SHM_SIZE; i++, ptr++) {
        *ptr = 97 + rand() % 26;
    }

    // Writing on spaces
    for (int i = 0; i < n_proc; i++, ptr += sizeof(int)) {
        *ptr = -1;
    }
    ptr -= n_proc * sizeof(int);

    int sum_occurrences;

    syncProcs:
    while (1) {
        sum_occurrences = 0;
        for (int i = 0; i < n_proc; i++, ptr += sizeof(int)) {
            if (*ptr == -1) continue syncProcs;
            else sum_occurrences += *ptr;
        }
        break;
    }

    printf("The sum of occurrences of %c is %d.\n", wanted_char,
        sum_occurrences);

    return 0;
}
