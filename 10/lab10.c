#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char* argv[]) {
	pid_t cpid, w;
	int wstatus;

	if (argc < 2) {
		fprintf(stderr, "Usage: %s command\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	if ((cpid = fork()) > 0) { // fork() => PARENT
		printf("Parent: waiting for child\n\n");

		do { w = wait(&wstatus); } while (w == -1 && errno == EINTR); // EINTR => wait() again
		
		if (w == -1) {
			perror("wait");
			exit(EXIT_FAILURE);
		}
		
		if (WIFEXITED(wstatus)) {
			printf("\nParent: child was exited with exit code %d\n", WEXITSTATUS(wstatus));
		} else if (WIFSIGNALED(wstatus)) {
			printf("\nParent: child was killed by signal %d\n", WTERMSIG(wstatus));
		}
	} else if (cpid == 0) { // fork() => CHILD
		execvp(argv[1], argv + 1);
		perror("execvp:");	
		exit(EXIT_FAILURE);
	} else { // fork() => ERROR
		perror("fork");
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}