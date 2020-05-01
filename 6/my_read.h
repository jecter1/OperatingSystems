#ifndef MY_READ_H
#define MY_READ_H

#include <unistd.h>
#include <stddef.h>

ssize_t my_read(int fd, void* buf, size_t count);

#endif