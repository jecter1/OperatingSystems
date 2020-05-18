#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
	pid_t cpid1, cpid2;

	// Inheritance: P1 <- P2 <- P3
	if ((cpid1 = fork()) > 0) { // P1 (otherwise terminal will idle)
		exit(EXIT_SUCCESS);
	} else if (cpid1 == 0) {
		if ((cpid2 = fork()) > 0) { // P2 => idle
			for (;;);
		} else if (cpid2 == 0) {  // P3 => zombie
			exit(EXIT_SUCCESS);
		} else {  // fork2 => ERROR
			perror("fork2");
			exit(EXIT_FAILURE);
		}
	} else { // fork1 => ERROR
		perror("fork1");
		exit(EXIT_FAILURE);
	}
}