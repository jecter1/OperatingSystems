#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFSIZE 32

int main() {
	FILE* pipein = popen("./lab26r", "w");
	if (pipein == NULL) {
		perror("popen");
		exit(EXIT_FAILURE);
	}

	char buf[BUFSIZE] = "HeLlO fRoM P1";
	if (fwrite(buf, sizeof(char), strlen(buf), pipein) < strlen(buf) && ferror(pipein)) {
		perror("fwrite");
		exit(EXIT_FAILURE);
	}
	printf(">P1 sent message to P2: \n\"%s\"\n", buf);

	int wstatus = pclose(pipein);
	if (WIFEXITED(wstatus) && WEXITSTATUS(wstatus) == EXIT_SUCCESS) {
		printf(">Success!\n");
		exit(EXIT_SUCCESS);
	}

	fprintf(stderr, "Something went wrong.\n");
	exit(EXIT_FAILURE);
}