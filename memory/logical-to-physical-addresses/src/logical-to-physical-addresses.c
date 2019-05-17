#include <math.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char const *argv[]) {

  // variables to be used
  int page_size, offset, offset_bits;
  long unsigned page;
  int success = 1, exit_program = 1;

  // getting information using the system's page size
  page_size = getpagesize();
  offset = sqrt(page_size);
  offset_bits = sqrt(offset);
  page = ((long unsigned)&success);

  // communicating with the user
  printf("Hi, I'm the process %d.\n", getpid());
  printf("The actual page size is: %d bytes. ", page_size);
  printf("That is 2^%d bytes.\n", offset);
  printf("So, the offset is %d bits.\n\n", (int)sqrt(offset));

  printf("The variable 'success' has the logical address: %p.\n",
         /*(unsigned int)*/&page_size);
  printf("So, the page physical address is %p.\n", vtop(page));

  while (exit_program) {
    scanf("%d", &exit_program);
  }

  return 0;
}
