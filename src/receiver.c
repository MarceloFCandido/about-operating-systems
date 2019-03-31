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

#define SHM_SIZE 1048576 // 1 MB
// #define SHM_SIZE 1073741824 // 1GB
#define SHM_NAME "shared_memory"

int main(int argc, char const *argv[]) {

    int shm_fd;
    char *ptr, *start;

    int id_proc = atoi(argv[1]);
    // printf("Receiver %d: Hello!\n", id_proc);

    // Opening shared memory created by sender
    shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);

    // Mapping the shared memory allocated by sender to the process's address
    // space
    int n_proc = atoi(argv[2]);
    ptr = mmap(0, 1 + n_proc * sizeof(int) + SHM_SIZE, PROT_READ | PROT_WRITE,
        MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        printf("Receiver %d: Map failed\n", id_proc);
        return -1;
    }

    // Now, we need to save some information and put the pointer in the place
    // it needs to read
    char wanted_char = *ptr; // coppin
    start = ptr;
    ptr++; // entering the spaces for process put its occurrences number
    for (int i = 0; i < n_proc; i++, ptr += sizeof(int)) {
        // Skipping the said spaces
    }
    // the number of characters each process will have to analyze
    long char_per_proc = SHM_SIZE / n_proc;
    // if SHM_SIZE % n_proc != 0
    int remainder_char = SHM_SIZE % n_proc;

    // printf("Receiver %d: pointing for my subtring\n", id_proc);
    for (int i = 0; i < id_proc * char_per_proc; i++, ptr += sizeof(char)) {}

    int wanted_char_counted = 0;
    // printf("Receiver %d: starting to count characters '%c' in my substring.\n",
    //             id_proc, wanted_char);
    if (id_proc != n_proc - 1) {
        for (int i = 0; i < char_per_proc; i++, ptr += sizeof(char)) {
            if (*ptr == wanted_char) { wanted_char_counted++; }
        }
    } else { // in the case to be the last process
        for (int i = 0; i < char_per_proc + remainder_char;
                i++, ptr += sizeof(char)) {
            if (*ptr == wanted_char) { wanted_char_counted++; }
        }
    }

    printf ("Receiver %d: counted %d %c's.\n",
            id_proc, wanted_char_counted, wanted_char);

    ptr = start;
    ptr++;
    for (int i = 0; i < id_proc; i++, ptr += sizeof(int)) {}
    *ptr = wanted_char_counted;
    printf("Receiver %d: %d in the pointer location.\n", id_proc, (int) *ptr);


    return 0;
}
