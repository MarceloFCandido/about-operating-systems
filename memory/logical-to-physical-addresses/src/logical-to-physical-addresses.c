#include <math.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char const *argv[]) {

  int page_size, offset, offset_bits;
  long unsigned int page;
  int exit_program = 1;

  page_size = getpagesize();
  offset = sqrt(page_size);
  offset_bits = sqrt(offset);
  page = ((int)&page_size) >> offset_bits;

  printf("Hi, I'm the process %d.\n", getpid());
  printf("The actual page size is: %d bytes. ", page_size);
  printf("That is 2^%d bytes.\n", offset);
  printf("So, the offset is %d bits.\n\n", (int)sqrt(offset));

  printf("The variable 'page_size' has the logical address: %d.\n",
         (unsigned int)&page_size);
  printf("So, the page address is %ld.\n", page);

  while (exit_program) {
    scanf("%d", &exit_program);
  }

  return 0;
}
