#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 256

int main(int argc, char* argv[]) {
	if (argc != 2) {
		fprintf(stderr, "Usage: %s filename\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	char cmd[BUFSIZE] = "{ cat ";
	strcat(cmd, argv[1]);
	strcat(cmd, "; echo $? > tmpfile; } | wc -l");

	FILE* pipeout = popen(cmd, "r");
	if (pipeout == NULL) {
		perror("popen");
		exit(EXIT_FAILURE);
	}

	int cnt;
	fscanf(pipeout, "%d", &cnt);

	int wstatus = pclose(pipeout);
	if (WIFEXITED(wstatus) && WEXITSTATUS(wstatus) == 0) {
		// cat exit code != 0 => failure
		FILE* tmp = fopen("tmpfile", "r");
		if (tmp == NULL) {
			perror("fopen");
			if (remove("tmpfile") != 0) {
				perror("remove");
			}
			exit(EXIT_FAILURE);
		}

		int cat_exit_code;
		fscanf(tmp, "%d", &cat_exit_code);

		if (fclose(tmp) != 0) {
			perror("fclose");
		}

		if (remove("tmpfile") != 0) {
			perror("remove");
		}

		if (cat_exit_code != 0) {
			fprintf(stderr, "Error: cat exited with exit code %d\n", cat_exit_code);
			exit(EXIT_FAILURE);
		}
		
		// Otherwise success
		printf(">Count of lines: %d\n", cnt);
		exit(EXIT_SUCCESS);
	} else if (WIFEXITED(wstatus)) {
		fprintf(stderr, "Error: wc exited with exit code %d\n", WEXITSTATUS(wstatus));
	} else if (WIFSIGNALED(wstatus)) {
		fprintf(stderr, "Error: wc was killed by signal %d\n", WTERMSIG(wstatus));
	}
	if (remove("tmpfile") != 0) {
		perror("remove");
	}
	exit(EXIT_FAILURE);
}