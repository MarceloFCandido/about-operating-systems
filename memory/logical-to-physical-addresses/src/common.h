#ifndef COMMON_H
#define COMMON_H

#define _XOPEN_SOURCE 700
#include <fcntl.h> /* open */
#include <math.h>  /* fabs */
#include <stdbool.h>
#include <stdint.h> /* uint64_t  */
#include <stdio.h>  /* snprintf */
#include <stdlib.h> /* size_t */
#include <sys/types.h>
#include <unistd.h> /* pread, sysconf */

/* Format documented at:
 * https://github.com/torvalds/linux/blob/v4.9/Documentation/vm/pagemap.txt
 */
typedef struct {
  uint64_t pfn : 54;
  unsigned int soft_dirty : 1;
  unsigned int file_page : 1;
  unsigned int swapped : 1;
  unsigned int present : 1;
} PagemapEntry;

/* Parse the pagemap entry for the given virtual address.
 *
 * @param[out] entry      the parsed entry
 * @param[in]  pagemap_fd file descriptor to an open /proc/pid/pagemap file
 * @param[in]  vaddr      virtual address to get entry for
 * @return                0 for success, 1 for failure
 */
int pagemap_get_entry(PagemapEntry *entry, int pagemap_fd, uintptr_t vaddr) {
  size_t nread;
  ssize_t ret;
  uint64_t data;
  uintptr_t vpn;

  vpn = vaddr / sysconf(_SC_PAGE_SIZE);
  nread = 0;
  while (nread < sizeof(data)) {
    // read from pagemap file to data buffer, sizeof(data) - nread bytes
    // with a offset of vpn * sizeof(data) + nread)
    ret = pread(pagemap_fd, &data, sizeof(data) - nread,
                vpn * sizeof(data) + nread);
    nread += ret;
    if (ret <= 0) {
      return 1;
    }
  }

  // parsing of the retrivied data as showed in the file above (of Linux
  // Torvalds)
  entry->pfn = data & (((uint64_t)1 << 54) - 1);
  entry->soft_dirty = (data >> 54) & 1;
  entry->file_page = (data >> 61) & 1;
  entry->swapped = (data >> 62) & 1;
  entry->present = (data >> 63) & 1;

  return 0;
}

/* Convert the given virtual address to physical using /proc/PID/pagemap.
 *
 * @param[out] paddr physical address
 * @param[in]  pid   process to convert for
 * @param[in]  vaddr virtual address to get entry for
 * @return           0 for success, 1 for failure
 */
int virt_to_phys_user(uintptr_t *paddr, pid_t pid, uintptr_t vaddr) {
  // creating a array, with the size of the buffer, to allocate the
  // pagemap file
  char pagemap_file[BUFSIZ];
  int pagemap_fd;

  // printing pagemap file (of the processor id) to the previous array.
  // It uses, the buffer size to tell how many bytes to read
  snprintf(pagemap_file, sizeof(pagemap_file), "/proc/%ju/pagemap",
           (uintmax_t)pid);
  pagemap_fd = open(pagemap_file, O_RDONLY);
  if (pagemap_fd < 0) {
    return 1;
  }
  PagemapEntry entry;
  // searching virtual address in the pagemap file
  if (pagemap_get_entry(&entry, pagemap_fd, vaddr)) {
    return 1;
  }
  // closing pagemap file
  close(pagemap_fd);

	// getting physical address
  *paddr =
      (entry.pfn * sysconf(_SC_PAGE_SIZE)) + (vaddr % sysconf(_SC_PAGE_SIZE));
  return 0;
}

#endif
