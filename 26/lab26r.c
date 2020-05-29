#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>

#define BUFSIZE 256

ssize_t read_while_EINTR(int fd, void* buf, size_t count) {
	ssize_t n;
	do { n = read(fd, buf, count); } while (n == -1 && errno == EINTR);
	return n;
}

int main() {
	char buf[BUFSIZE] = "";
	if (read_while_EINTR(STDIN_FILENO, buf, BUFSIZE) < 0) {
		perror("read");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < strlen(buf); ++i) {
		buf[i] = toupper((unsigned char) buf[i]);
	}

	printf("Proc2 recieved message from proc1: %s\n", buf);
}