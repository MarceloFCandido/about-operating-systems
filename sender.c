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

    if (argc < 3) {
        printf("Usage: %s n_procs wanted_char\n", argv[0]);
        return 1;
    }

    int n_proc; // number of receivers
    char wanted_char; // character to be searched

    n_proc = atoi(argv[1]);
    wanted_char = argv[2][0];

    // Creating shared memory for communication between processes
    shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);

    // Defining shared memory space
    ftruncate(shm_fd, 1 + n_proc * sizeof(int) + SHM_SIZE);

    // Mapping the shared memory allocated to the process's address space
    ptr = mmap(0, 1 + n_proc * sizeof(int) + SHM_SIZE, PROT_READ | PROT_WRITE,
        MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        printf("Map failed\n");
        return -1;
    }

    // Save start position
    start = ptr;

    // Write wanted_char in the first position 
    *(ptr++) = wanted_char;

    // Writing on spaces
    for (int i = 0; i < n_proc; i++, ptr += sizeof(int)) {
        *ptr = -1;
    }

    // Writing randomly in the shared memory
    for (int i = 0; i < SHM_SIZE; i++, ptr++) {
        *ptr = 97 + rand() % 26;
    }

    int sum_occurrences;

    printf("Waiting for sum to be finished...\n");
    while (1) {
        ptr = start + 1;
        sum_occurrences = 0;

        int sum_finished = 1;
        for (int i = 0; i < n_proc; i++, ptr += sizeof(int)) {
            if (*ptr == -1) {
                sum_finished = 0;
                break;
            } else sum_occurrences += *ptr;
        }

        if (sum_finished) break;
    }

    printf("The sum of occurrences of %c is %d.\n", wanted_char,
        sum_occurrences);

    return 0;
}
