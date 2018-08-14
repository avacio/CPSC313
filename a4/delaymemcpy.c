#include "delaymemcpy.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <stdint.h>

#define MAX_PENDING_COPIES 100

/* Data structure use to keep a list of pending memory copies. */
typedef struct pending_copy {

  /* Flag to indicate if this pending copy object is in use or free */
  unsigned int in_use: 1;

  /* Source, destination and size of the memory regions involved in the copy */
  void *src;
  void *dst;
  size_t size;

  /* Next pending copy. NULL if there is no other pending copy */
  struct pending_copy *next;

} pending_copy_t;

/* First element of the pending copy linked list. The order of the
 * list matters: if two or more copies have overlapping pages, they
 * must be performed in the order of the list.
 */
static pending_copy_t *first_pending_copy = NULL;

/* Array of possible pending copies pointers. Because malloc/free are
   not async-safe they cannot safely be called inside a signal
   handler, so an array of objects is created, and the in_use flag is
   used to determine which objects are in use.
 */
static pending_copy_t pending_copy_slots[MAX_PENDING_COPIES];

/* Global variable to keep the current page size. Initialized in
   initialize_delay_memcpy_data.
 */
static long page_size = 0;

/* Returns the pointer to the start of the page that contains the
   specified memory address.
 */
static void *page_start(void *ptr) {

  return (void *) (((intptr_t) ptr) & -page_size);
}

/* Returns TRUE (non-zero) if the address 'ptr' is in the range of
   addresses that starts at 'start' and has 'size' bytes (i.e., ends at
   'start+size-1'). Returns FALSE (zero) otherwise.
 */
static inline int address_in_range(void *start, size_t size, void *ptr) {

  /* TO BE COMPLETED BY THE STUDENT */
  if (ptr < start || ptr > start + size - 1) {
    return 0;
  }
  return 1;
}

/* Returns TRUE (non-zero) if the addresses 'ptr1' and 'ptr2' are in
   the same page. Returns FALSE (zero) otherwise.
 */
static inline int address_in_same_page(void *ptr1, void *ptr2) {

  /* TO BE COMPLETED BY THE STUDENT */
  return address_in_range(page_start(ptr1), page_size, ptr1) == address_in_range(page_start(ptr2), page_size, ptr2);
}

/* Returns TRUE (non-zero) if the address 'ptr' is in the same page as
   any address in the range of addresses that starts at 'start' and
   has 'size' bytes (i.e., ends at 'start+size-1'). In other words,
   returns TRUE if the address is in the range [ps, pe], where ps is
   the start of the first page in the range, and pe is the end of the
   last page in the range. Returns FALSE (zero) otherwise.
 */
static inline int address_in_same_pages(void *start, size_t size, void *ptr) {

  /* TO BE COMPLETED BY THE STUDENT */
  return ((address_in_range(start, size, ptr))
          || (address_in_range(page_start(start), page_size, ptr))
          || (address_in_range(page_start(start) + size - 1, page_size, ptr)));

  //loop? keep adding size until at start+size-1
}

/* mprotect requires the start address to be aligned with the page
   size. This function calls mprotect with the start of the page that
   contains ptr, and adjusts the size accordingly to include the extra
   bytes required for that adjustment.

   Obs: according to POSIX documentation, mprotect cannot safely be
   called inside a signal handler. However, in most modern Unix-based
   systems (including Linux), mprotect is async-safe, so it can be
   called without problems.
 */
static inline int mprotect_full_page(void *ptr, size_t size, int prot) {

  void *page = page_start(ptr);
  return mprotect(page, size + (ptr - page), prot);
}

// Protects all pending copy pages by setting src to read only, and dst as no access
void mprotect_all() {
  pending_copy_t *curr = first_pending_copy;
  while (curr != NULL) {
    mprotect_full_page(curr->src, curr->size, PROT_READ);
    mprotect_full_page(curr->dst, curr->size, PROT_NONE);
    curr = curr->next;
  }
}

/* Changes the permission of the pages to allow them to be copied,
   then performs the actual copy.
 */
static inline void actual_copy(void *dst, void *src, size_t size) {

  /* TO BE COMPLETED BY THE STUDENT */
  mprotect_full_page(src, size, PROT_READ | PROT_WRITE);
  mprotect_full_page(dst, size, PROT_READ | PROT_WRITE);
  memcpy(dst, src, size);
}

/* Adds a pending copy object to the list of pending copies. If
   add_after is NULL, adds the object to the end of the list,
   otherwise adds it right after add_after.
 */
static void add_pending_copy(void *dst, void *src, size_t size, pending_copy_t *add_after) {

  int i;

  // Find an available copy object slot. This would usually be done
  // with a call to malloc, but since malloc cannot safely be called
  // inside a signal handler, this is done with an array of
  // "pre-allocated" objects.
  pending_copy_t *new_copy = NULL;
  for (i = 0; i < MAX_PENDING_COPIES; i++) {
    if (!pending_copy_slots[i].in_use) {
      new_copy = &pending_copy_slots[i];
      break;
    }
  }

  // If there is no available copy, force the oldest copy object to be copied to make room
  if (new_copy == NULL) {
    actual_copy(first_pending_copy->dst, first_pending_copy->src, first_pending_copy->size);
    new_copy = first_pending_copy;
    first_pending_copy = new_copy->next;
  }

  if (!add_after && first_pending_copy)
    for (add_after = first_pending_copy; add_after->next; add_after = add_after->next);

  new_copy->src = src;
  new_copy->dst = dst;
  new_copy->size = size;
  new_copy->in_use = 1;

  if (add_after == new_copy) {
    // The base copy is the old slot of the current value, so add to start
    new_copy->next = first_pending_copy;
    first_pending_copy = new_copy;
  }
  else if (add_after) {
    // There is at least one previous pending copy, add element after it
    new_copy->next = add_after->next;
    add_after->next = new_copy;
  }
  else {
    // The list is currently empty, add it to start
    new_copy->next = NULL;
    first_pending_copy = new_copy;
  }
}

/* Removes a pending copy object from the list of pending copies.
 */
static void remove_pending_copy(pending_copy_t *copy) {

  pending_copy_t *prev = NULL;

  if (!copy) return;

  if (copy == first_pending_copy)
    first_pending_copy = copy->next;
  else {

    for (prev = first_pending_copy; prev && prev->next && prev->next != copy; prev = prev->next);
    if (prev)
      prev->next = copy->next;
  }

  copy->in_use = 0;
}

/* Removes part of a pending copy object from the list of pending
   copies. In essence this process replaces the existing pending copy
   with up to two copies containing the ranges before and after the
   affected range.
*/
static inline void remove_pending_copy_range(pending_copy_t *copy, void *src, size_t size) {

  // Calculates the size of the 'before' range.
  ssize_t before_size = src - copy->src;
  // Calculates how many bytes ahead of the start of the copy range
  // the 'after' range is found.
  size_t after_delta = before_size + size;

  if (before_size > 0) {
    if (after_delta < copy->size)
      add_pending_copy(copy->dst + after_delta, copy->src + after_delta, copy->size - after_delta, copy);
    copy->size = before_size;
  }
  else if (after_delta < copy->size) {
    copy->dst += after_delta;
    copy->src += after_delta;
    copy->size -= after_delta;
  }
  else
    remove_pending_copy(copy);
}

/* Returns the oldest pending copy object for which the source or
   destination range contains the provided address, either within the
   range itself, or in the same page in the page table. Returns NULL
   if no such object exists in the list.
 */
static pending_copy_t *get_pending_copy(void *ptr) {

  pending_copy_t *copy;

  for (copy = first_pending_copy; copy; copy = copy->next) {

    if (address_in_same_pages(copy->src, copy->size, ptr) ||
        address_in_same_pages(copy->dst, copy->size, ptr))
      return copy;
  }
  return NULL;
}

/* Segmentation fault handler. If the address that caused the
   segmentation fault (represented by info->si_addr) is part of a
   pending copy, this function will perform the copy for the entire
   page that contains the address. If the pending copy corresponding
   to the address involves more than one page, the object will be
   replaced with the remaining pages (before and after the affected
   page), otherwise it will be removed from the list. If the address
   is not part of a pending copy page, the process will write a
   message to the standard error (stderr) output and kill the process.
 */
static void delay_memcpy_segv_handler(int signum, siginfo_t *info, void *context) {

  void *accessed_address = info->si_addr;
  pending_copy_t *copy = get_pending_copy(accessed_address);
  if (copy == NULL) {
    // Resume default behaviour for segfault
    struct sigaction sa;
    sa.sa_handler = SIG_DFL;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGSEGV, &sa, NULL);

    // Returning at this point will cause the memory operation to be
    // repeated, which will cause segfault to be handled by the
    // default behaviour associated to it by the OS.
    return;
  }

  /* TO BE COMPLETED BY THE STUDENT */

  // case 1: both source and destination correspond to exactly one page of data
  if (page_start(copy->src) == page_start(copy->dst)
      && page_start(copy->src) == page_start(copy->dst + copy->size - 1)
      && page_start(copy->dst) == page_start(copy->src + copy->size - 1)) {

    // printf("case 1\n");
    actual_copy(copy->dst, copy->src, copy->size);
    remove_pending_copy(copy);
    return;
  }

  // case: source and destination contain multiple full pages
  void *ptr1, *ptr2, *ptrCopy1, *ptrCopy2;

  // accessed_address is within the pages
  if (address_in_same_pages(copy->src, copy->size, accessed_address)) {
    ptr1 = copy->src;
    ptr2 = copy->dst;
  } else {
    ptr2 = copy->src;
    ptr1 = copy->dst;
  }

  int beforePage = 0; // is 1 if the ptr starts before page
  int afterPage = 0;  // is 1 if the ptr starts after page
  size_t origSize = copy->size;
  size_t newSize = copy->size;

  // case: source and/or destination are not aligned to page boundaries
  // ptr1 points before page begins
  if (ptr1 < page_start(accessed_address)) {
    ptrCopy1 = page_start(accessed_address);
    size_t pageOffset = ptrCopy1 - ptr1;
    newSize = origSize-pageOffset;
    ptrCopy2 = ptr2+pageOffset;
    beforePage = 1;
  } else { // ptr1 within page
    ptrCopy1 = ptr1;
    ptrCopy2 = ptr2;
  }

  // ptr1 points to after page ends
  if (ptr1+origSize > page_start(accessed_address)+page_size) {
    newSize = page_start(accessed_address) + page_size - ptrCopy1;
    afterPage = 1;
  }

  // ptr2 points to after page ends
  if (ptrCopy2+newSize-1 > page_start(ptrCopy2+page_size)) {
    newSize = page_start(ptrCopy2) + page_size - ptrCopy2;
    afterPage = 1;
  }

  while (get_pending_copy(ptrCopy2) != NULL && get_pending_copy(ptrCopy2) != copy) {
    accessed_address = ptrCopy2;
    delay_memcpy_segv_handler(0, info, NULL); // recurse through pending copies
  }

  // perform the copy
  void *dstCopy, *srcCopy;
  if (copy->src != ptr1) {
    actual_copy(ptrCopy1, ptrCopy2, newSize);
    dstCopy = ptrCopy1;
    srcCopy = ptrCopy2;
  } else {
    actual_copy(ptrCopy2, ptrCopy1, newSize);
    dstCopy = ptrCopy2;
    srcCopy = ptrCopy1;
  }

  if (beforePage && afterPage) {
    // printf("pointers before and after page bounds\n");
    copy->size = dstCopy - copy->dst;
    mprotect_all();
    add_pending_copy(dstCopy-newSize, srcCopy+newSize, origSize-newSize-copy->size, copy);
  } else if (beforePage) {
    // printf("pointer before page bounds\n");
    copy->size = dstCopy - copy->dst;
  } else if (afterPage) {
    // printf("pointer after page bounds\n");
    copy->src = copy->src + newSize;
    copy->dst = copy->dst + newSize;
    copy->size = origSize - newSize;
  } else {
    remove_pending_copy(copy); // regular copy, so remove from pending list
  }
  mprotect_all();
}

/* Initializes the data structures and global variables used in the
   delay memcpy process. Changes the signal handler for segmentation
   fault to the handler used by this process.
 */
void initialize_delay_memcpy_data(void) {

  struct sigaction sa;
  sa.sa_sigaction = delay_memcpy_segv_handler;
  sa.sa_flags = SA_SIGINFO;
  sigemptyset(&sa.sa_mask);
  sigaction(SIGSEGV, &sa, NULL);

  page_size = sysconf(_SC_PAGESIZE);
}

/* Starts the copying process of 'size' bytes from 'src' to 'dst'. The
   actual copy of data is performed in the signal handler for
   segmentation fault. This function only stores the information
   related to the copy in the internal data structure, and protects
   the pages (source as read-only if not protected further,
   destination as no access) so that the signal handler is invoked
   when the copied data is needed. Returns the value of dst.
 */
void *delay_memcpy(void *dst, void *src, size_t size) {

  /* TO BE COMPLETED BY THE STUDENT */
  add_pending_copy(dst, src, size, NULL);
  mprotect_full_page(src, size, PROT_READ);
  mprotect_full_page(dst, size, PROT_NONE);

  mprotect_all();
  return dst;
}
