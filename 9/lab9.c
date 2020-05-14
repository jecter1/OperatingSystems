#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
	pid_t cpid, w;
	int wstatus;

	if ((cpid = fork()) > 0) { 
		// parent
		printf("Parent: waiting for child\n\n");
		w = wait(&wstatus);
		if (w == -1) {
			perror("wait");
			exit(EXIT_FAILURE);
		}
		printf("\nParent: child terminated\n");
	} else if (cpid == 0) { 
		// child
		execl("/bin/cat", "cat", "text", (char *) NULL);
		perror("execl:");
		exit(EXIT_FAILURE);
	} else { 
		// error
		perror("fork");
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}