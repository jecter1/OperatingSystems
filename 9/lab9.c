#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

int main() {
	pid_t cpid, w;
	int wstatus;

	if ((cpid = fork()) > 0) { // PARENT
		printf("Parent: waiting for child\n\n");

		do { w = wait(&wstatus); } while (w == -1 && errno == EINTR); // EINTR => wait() again

		if (w == -1) {
			perror("wait");
			exit(EXIT_FAILURE);
		}

		printf("\nParent: child terminated\n");
	} else if (cpid == 0) { // CHILD
		execl("/bin/cat", "cat", "text", (char *) NULL);
		perror("execl");
		exit(EXIT_FAILURE);
	} else { // ERROR
		perror("fork");
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}