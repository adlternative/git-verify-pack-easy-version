#ifndef ADL_FILE_UTIL_H__
#define ADL_FILE_UTIL_H__

#include <errno.h>
#include <unistd.h>

size_t get_size(int fd);
ssize_t read_n(int fd, const char *buf, size_t len);

#endif // ADL_FILE_UTIL_H__