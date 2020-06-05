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
	if (wstatus == -1) {
		// pclose() error => failure
		perror("pclose");
		exit(EXIT_FAILURE);
	if (WIFEXITED(wstatus) && WEXITSTATUS(wstatus) == EXIT_SUCCESS) {
		// lab26r success => success
		exit(EXIT_SUCCESS);
	} else if (WIFEXITED(wstatus)) {
		// lab26r exit code != 0 => failure
		fprintf(stderr, "Error: P2 exited with exit code %d\n", WEXITSTATUS(wstatus));
	} else if (WIFSIGNALED(wstatus)) {
		// lab26r killed by signal => failure
		fprintf(stderr, "Error: P2 was killed by signal %d\n", WTERMSIG(wstatus));
	}
	exit(EXIT_FAILURE);
}