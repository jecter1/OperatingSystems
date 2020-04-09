#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

main() {
	FILE *fp;
	int err;

	err = printf("UID: %d\nEUID: %d\n", getuid(), geteuid());
	if (err < 0) {
		perror("Writing UID/EUID [1]");
		exit(1);
	}

	if ((fp = fopen("file", "r")) == NULL) {
		perror("Opening file [1]");
	}
	else if (fclose(fp) != 0) {
		perror("Closing file [1]");
		exit(1);
	}

	if (seteuid(getuid()) != 0) {
		perror("Setting UID");
		exit(1);
	}

	err = printf("UID: %d\nEUID: %d\n", getuid(), geteuid());
	if (err < 0) {
		perror("Writing UID/EUID [2]");
		exit(1);
	}

	if ((fp = fopen("file", "r")) == NULL) {
		perror("Opening file [2]");
	}
	else if (fclose(fp) != 0) {
		perror("Closing file [2]");
		exit(1);
	}

	exit(0);
}
