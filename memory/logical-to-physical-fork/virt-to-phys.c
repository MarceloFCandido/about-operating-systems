/* https://github.com/cirosantilli/linux-kernel-module-cheat#userland-physical-address-experiments
 */

#define _XOPEN_SOURCE 700
#include <stdio.h>  /* printf */
#include <stdlib.h> /* EXIT_SUCCESS, EXIT_FAILURE, strtoull */
#include <unistd.h> 
#include "common.h" /* virt_to_phys_user */

int main(int argc, char **argv) {
  pid_t pid;
  uintptr_t vaddr, paddr = 0;
  fork();
  if (virt_to_phys_user(&paddr, getpid(), vaddr)) {
    fprintf(stderr, "error: virt_to_phys_user\n");
    return EXIT_FAILURE;
  };
  printf("Lógico: 0x%jx\n", (uintmax_t)vaddr);
  printf("Físico: 0x%jx\n", (uintmax_t)paddr);
  return EXIT_SUCCESS;
}
