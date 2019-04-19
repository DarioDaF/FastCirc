#ifndef _FASTCIRC_H_
#define _FASTCIRC_H_

#include <stdlib.h>

struct fastcirc {
  int fd;
  size_t size;
  void* buf;
};

struct fastcirc fastcirc_alloc(size_t size);
void fastcirc_free(struct fastcirc* fc);

#endif
