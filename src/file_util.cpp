
#include "file_util.hpp"
#include <cstddef>
#include <sys/stat.h>


size_t get_size(int fd) {
  struct stat st;
  int ret = fstat(fd, &st);
  if (ret)
    return -1;
  return st.st_size;
}

ssize_t read_n(int fd, const char *buf, size_t len) {
  ssize_t n = 0;
  while (n < len) {
    ssize_t r = read(fd, (void *)(buf + n), len - n);
    if (r < 0) {
      if (errno == EINTR) {
        continue;
      }
      return -1;
    }
    if (r == 0) {
      return n;
    }
    n += r;
  }
  return n;
}
