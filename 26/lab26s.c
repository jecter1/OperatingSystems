#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFSIZE 256

int pclose_while_EINTR(FILE* pipe) {
	int n;
	do { n = pclose(pipe); } while (n == -1 && errno == EINTR);
	return n;
}

int main() {
	FILE* pipein = popen("./lab26r", "w");
	if (pipein == NULL) {
		perror("popen");
		exit(EXIT_FAILURE);
	}

	char buf[BUFSIZE] = "HeLlO fRoM cHiLd1";
	if (fwrite(buf, sizeof(char), strlen(buf), pipein) < strlen(buf)) {
		perror("fwrite");
		exit(EXIT_FAILURE);
	}
	printf("Proc1 sent message to proc2: %s\n", buf);

	if (pclose_while_EINTR(pipein) == -1) {
		perror("pclose");
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}