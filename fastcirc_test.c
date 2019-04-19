#include <stdlib.h>
#include <stdio.h>

#include <string.h>
#include <assert.h>

#include "fastcirc.h"

// Must be odd! (or simply not multiple of page size)
#define SIZE 101

int main() {

  struct fastcirc fc = fastcirc_alloc(SIZE);
  printf("Fast Circular Buffer created: size = %lu (%d)\n", fc.size, SIZE);

  printf("Preparing array...\n");
  unsigned char arr[SIZE];
  for(int i = 0; i < SIZE; ++i) {
    arr[i] = i & 0xFF;
  }

  printf("Filling buffer...\n");
  for(void* buf = fc.buf; buf < fc.buf + fc.size; /* Do the increment INSIDE */) {
    buf += SIZE;
    memcpy(buf, arr, SIZE);
  }

  int rem = fc.size % SIZE;
  printf("Checking overlap of %d bytes...\n", SIZE - rem);
  assert(rem > 0);
  for(int i = rem; i < SIZE; ++i) {
    assert(((unsigned char*)fc.buf)[i - rem] == i & 0xFF);
  }

  printf("Done\n");
  return EXIT_SUCCESS;
}
