#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "delaymemcpy.h"

#define MAX_ARRAY_SIZE 0x40000000 // 2^30=1GB

/* Arrays declared as global, as this allows program to use more
   memory. Aligned to page boundaries so other global variables don't
   end up in same page. */
unsigned char __attribute__ ((aligned (0x1000))) array1[MAX_ARRAY_SIZE];
unsigned char __attribute__ ((aligned (0x1000))) array2[MAX_ARRAY_SIZE];

/* Initializes the array with a random set of values. */
void *random_array(void *array, size_t size) {

  int i;
  size /= sizeof(long);
  for (i = 0; i < size; i++)
    ((long *)array)[i] = random();

  return array;
}

/* Prints all values of the array */
void print_array(unsigned char *array, int size) {
  int i;
  for (i = 0; i < size; i++)
    printf("%02x ", array[i]);
  printf("\n");
}

int main(void) {

  srandom(time(NULL));

  initialize_delay_memcpy_data();

  printf("\nCopying one page of data, trigger copy via read dst...\n");
  random_array(array1, 0x1000);
  printf("Before copy: ");
  print_array(array1, 20);

  delay_memcpy(array2, array1, 0x1000);
  printf("After copy : ");
  print_array(array1, 20); // Should not trigger copy
  printf("Destination: ");
  print_array(array2, 20);  // Triggers copy

  printf("\nCopying one page of data, trigger copy via write dst...\n");
  random_array(array1, 0x1000);
  printf("Before copy: ");
  print_array(array1, 20);

  delay_memcpy(array2, array1, 0x1000);
  array2[0] = random(); // Triggers copy
  printf("After copy : ");
  print_array(array1, 20);
  printf("Destination: ");
  print_array(array2, 20);

  printf("\nCopying one page of data, trigger copy via write src...\n");
  random_array(array1, 0x1000);
  printf("Before copy: ");
  print_array(array1, 20);

  delay_memcpy(array2, array1, 0x1000);
  array1[0] = random(); // Triggers copy
  printf("After copy : ");
  print_array(array1, 20);
  printf("Destination: ");
  print_array(array2, 20);

  printf("\nCopying two pages of data...\n");
  random_array(array1, 0x2000);
  printf("Before copy: ");
  print_array(array1, 20);

  delay_memcpy(array2, array1, 0x2000);
  printf("After copy : ");
  print_array(array1, 20);
  printf("Destination: ");
  print_array(array2, 20);  // Triggers copy of first page only
  printf("2nd page:\nAfter copy : ");
  print_array(array1 + 0x1800, 20);
  printf("Destination: ");
  print_array(array2 + 0x1800, 20);  // Triggers copy of second page

  printf("\nCopying unaligned page of data...\n");
  random_array(array1, 0x2000);
  printf("Before copy: ");
  print_array(array1 + 0x400, 20);

  delay_memcpy(array2 + 0x400, array1 + 0x400, 0x1000);
  printf("After copy : ");
  print_array(array1 + 0x400, 20);
  printf("Destination: ");
  print_array(array2 + 0x400, 20);  // Triggers copy of first page only

  /* MORE TESTS COME HERE */

  // multi-page unaligned pages

  printf("\nCopying longer unaligned pages of data...\n");
  random_array(array1, 0x3000);
  printf("Before copy: ");
  print_array(array1 + 0x400, 30);

  delay_memcpy(array2 + 0x400, array1 + 0x400, 0x1000);
  printf("After copy : ");
  print_array(array1 + 0x400, 30);
  printf("Destination: ");
  print_array(array2 + 0x400, 30);  // Triggers copy of first page only
  printf("2nd page:\nAfter copy : ");
  print_array(array1 + 0x400, 30);
  printf("Destination: ");
  print_array(array2 + 0x400, 30);  // Triggers copy of second page


  printf("\nCopying shorter pages of data...\n");
  random_array(array1, 0x3000);
  printf("Before copy: ");
  print_array(array1, 10);

  delay_memcpy(array2, array1, 0x3000);
  printf("After copy : ");
  print_array(array1, 10);
  printf("Destination: ");
  print_array(array2, 10);  // Triggers copy of first page only
  printf("2nd page:\nAfter copy : ");
  print_array(array1 + 0x1800, 10);
  printf("Destination: ");
  print_array(array2 + 0x1800, 10);  // Triggers copy of second page

  printf("\nTesting other source of segfault...\n");
  volatile long *ptr = NULL;
  (long) *ptr;

  return 0;
}
