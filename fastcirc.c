#include "fastcirc.h"

#include <assert.h>
#define ASSERT(cond, msg) assert(cond)

#include <unistd.h>
#include <sys/syscall.h>
#include <sys/mman.h>

#define ROUNDN(x, n) (((x) / (n)) * (n))

int memfd_create(const char* name, unsigned int flags) {
    return syscall(SYS_memfd_create, name, flags);
}

struct fastcirc fastcirc_alloc(size_t size) {
  struct fastcirc fc;

  int pg_size = getpagesize();
  ASSERT(pg_size > 0, "Invalid page size");
  fc.size = ROUNDN(size + pg_size - 1, pg_size);

  fc.fd = memfd_create("fastcirc", 0);
  ASSERT(fc.fd > 0, "Unable to create memory file");
  ftruncate(fc.fd, size);

  fc.buf = mmap(NULL, 2 * fc.size, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  ASSERT(fc.buf != MAP_FAILED, "Unable to allocate memory map");

  ASSERT(
    mmap(fc.buf, fc.size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_FIXED, fc.fd, 0) != MAP_FAILED,
    "Unable to map file");
  ASSERT(
    mmap(fc.buf + fc.size, fc.size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_FIXED, fc.fd, 0) != MAP_FAILED,
    "Unable to map clone");

  return fc;
}

void fastcirc_free(struct fastcirc* fc) {
  close(fc->fd);
  fc->fd = -1;
  munmap(fc->buf, 2 * fc->size);
  fc->size = 0;
  fc->buf = NULL;
}
