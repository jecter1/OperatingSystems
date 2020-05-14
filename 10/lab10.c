#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
	pid_t cpid, w;
	int wstatus;

	if (argc < 2) {
		fprintf(stderr, "Usage: %s command\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	if ((cpid = fork()) > 0) {
		// parent
		printf("Parent: waiting for child\n\n");
		w = wait(&wstatus);
		if (w == -1) {
			perror("wait");
			exit(EXIT_FAILURE);
		}
		if (WIFEXITED(wstatus)) {
			printf("\nParent: child was exited with exit code %d\n", WEXITSTATUS(wstatus));
		} else if (WIFSIGNALED(wstatus)) {
			printf("\nParent: child was killed by signal %d\n", WTERMSIG(wstatus));
		}
	} else if (cpid == 0) {
		// child
		char** arguments = (char**)malloc(sizeof(char*) * (argc + 2));
		if (arguments == NULL) {
			perror("malloc");
			exit(EXIT_FAILURE);
		}

		arguments[0] = strdup("sh");
		if (arguments[0] == NULL) {
			perror("strdup");
			free(arguments);
			exit(EXIT_FAILURE);
		}

		arguments[1] = strdup("-c");
		if (arguments[1] == NULL) {
			perror("strdup");
			free(arguments[0]);
			free(arguments);
			exit(EXIT_FAILURE);
		}

		for (int i = 0; i < argc; ++i) {
			arguments[i + 2] = argv[i + 1];
		}

		execvp("sh", arguments);

		perror("execvp:");
		free(arguments[0]);
		free(arguments[1]);
		free(arguments);
		exit(EXIT_FAILURE);
	} else {
		// error
		perror("fork");
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}