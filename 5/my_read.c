#include "my_read.h"
#include <errno.h>

ssize_t my_read(int fd, void* buf, size_t count) {
	ssize_t n;
	do { n = read(fd, buf, count); } while (n == -1 && errno == EINTR);
	return n;
}