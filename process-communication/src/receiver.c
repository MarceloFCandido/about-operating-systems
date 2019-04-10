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

#define SHM_SIZE 1024 * 1024 * 1024 // 1 GB
#define SHM_NAME "shared_memory"

int main(int argc, char const *argv[]) {

    int shm_fd;
    char *ptr, *start, *sum_ptr;

    if (argc != 3) {
        printf("Usage: %s id_proc n_procs\n", argv[0]);
        return 1;
    }

    int id_proc = atoi(argv[1]);
    int n_procs = atoi(argv[2]);

    // Opening shared memory created by sender
    shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);

    // Mapping the shared memory allocated by sender to the process's address
    // space
    ptr = mmap(0, 1 + n_procs * sizeof(int) + SHM_SIZE, PROT_READ | PROT_WRITE,
        MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        printf("Receiver %d: Map failed\n", id_proc);
        return -1;
    }

    // Now, we need to save some information and put the pointer in the place
    // it needs to read
    start = ptr;
    char wanted_char = *(ptr++); // copying wanted_char from shared memory
    ptr += n_procs * sizeof(int); /// skipping control bytes

    int count_wanted_chars = 0;
    sum_ptr = start + 1 + (id_proc * sizeof(int)); // ptr to store final result

    int chars_per_proc = SHM_SIZE / n_procs;

    // Based on id_proc and n_procs, count chars in shared memory positions
    int start_position = id_proc * chars_per_proc;
    int end_position = (id_proc + 1) * chars_per_proc;

    if (id_proc == (n_procs - 1)) {
        end_position = SHM_SIZE; // in case it's the last, count remainder too
    }

    for (int i = start_position; i < end_position; i++) {
        if (ptr[i] == wanted_char) count_wanted_chars++;
    }

    printf ("Receiver %d: counted %d %c's.\n",
            id_proc, count_wanted_chars, wanted_char);

    // Putting the sum in the appropiated space
    *((int *) sum_ptr) = count_wanted_chars;

    return 0;
}
