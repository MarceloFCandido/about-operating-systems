#include <math.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char const *argv[]) {

  int page_size, offset, offset_bits;
  long int page;

  page_size = getpagesize();
  offset = sqrt(page_size);
  offset_bits = sqrt(offset);
  page = ((int) &page_size) >> offset_bits;

  printf("The actual page size is: %d bytes.\n", page_size);
  printf("That is 2^%d bytes.\n", offset);
  printf("So, the offset is %d bits.\n", (int) sqrt(offset));

  printf("The variable 'page_size' has the logical address: .%p\n", &page_size);
  printf("So, the page address is %ld.\n", page);

  return 0;
}
