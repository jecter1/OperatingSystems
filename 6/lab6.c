#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include <signal.h>
#include "table.h"
#include "my_read.h"

#define BUFF_SIZE 1024
#define ERROR_VAL -1

int isNumber(char* str, int len);

int printContent(int fd);

int main(int argc, char** argv) {
	if (argc != 2) {
		fprintf(stderr, "Usage: %s filename\n", argv[0]);
		return ERROR_VAL;
	} // incorrect usage

	// OPENING FILE
	int fd = open(argv[1], O_RDONLY);
	if (fd == -1) {
		fprintf(stderr, "Opening '%s': %s\n", argv[1], strerror(errno));
		return ERROR_VAL;
	}

	// MAKING TABLE
	Table* table = makeTable(fd);
	if (table == NULL) {
		perror("Making table");
		if (close(fd)) {
			fprintf(stderr, "Closing '%s': %s\n", argv[1], strerror(errno));
		}
		return ERROR_VAL;
	}

	printf("Enter number of line (1-%d) to see this line", table->size);
	printf(" / 0 to quit / wait 5 seconds to see full content of '%s':\n", argv[1]);
	
	// WAITING FOR INPUT WITHIN 5 SECONDS
	struct pollfd fds[1];
	fds[0].fd = 0;
	fds[0].events = POLLIN;

	int ready = poll(fds, 1, 5000);
	if (ready == -1) { // error
		perror("Polling");
		free(table);
		if (close(fd)) {
			fprintf(stderr, "Closing '%s': %s\n", argv[1], strerror(errno));
		}
		return ERROR_VAL;
	} else if (ready == 0) { // timeout
		if (printContent(fd) < 0) {
			perror("Printing content");
			free(table);
			if (close(fd)) {
				fprintf(stderr, "Closing '%s': %s\n", argv[1], strerror(errno));
			}
			return ERROR_VAL;
		}
		printf("\n");
		free(table);
		if (close(fd)) {
			fprintf(stderr, "Closing '%s': %s\n", argv[1], strerror(errno));
			return ERROR_VAL;
		}
		return 0;
	}

	// READING NUMBER & PRINTING LINE
	char str[BUFSIZ];
	int num;
	int printLineVal;

	for(;;) {
		scanf("%s", str);

		if (!isNumber(str, strlen(str))) {
			printf("This is not a number\n");
			continue;
		}

		if ((num = atoi(str)) == 0) { // user printed 0 (quit)
			break;
		}

		printLineVal = printLine(fd, table, num);
		if (printLineVal == -1) {
			printf("%d is out of range (1-%d)\n", num, table->size);
			continue;
		} else if (printLineVal < -1) {
			perror("Printing line");
			free(table);
			if (close(fd)) {
				fprintf(stderr, "Closing '%s': %s\n", argv[1], strerror(errno));
			}	
			return ERROR_VAL;
		}
	}

	free(table);
	if (close(fd)) {
		fprintf(stderr, "Closing '%s': %s\n", argv[1], strerror(errno));
		return ERROR_VAL;
	}
	return 0;
}

int isNumber(char* str, int len) {
	if (str == NULL) {
		fprintf(stderr, "isNumber(char* str, int len). str is NULL!\n");
		exit(ERROR_VAL);
	} // incorrect usage
	if (len < 0) {
		fprintf(stderr, "isNumber(char* str, int len). len < 0!\n");
		exit(ERROR_VAL);
	} // incorrect usage

	if (!(str[0] >= '0' && str[0] <= '9') && str[0] != '-' && str[0] != '+') {
		return 0;
	} // examlpe: "a..."

	if ((str[0] == '-' || str[0] == '+') && len == 1) {
		return 0;
	} // example: "-"

	for (int i = 1; (i < len) && isNumber; i++) {
		if (!(str[i] >= '0' && str[i] <= '9')) {
			return 0;
		}
	} // example: "-124a..."
	
	return 1;
}

int printContent(int fd) {
	if (fd < 0) {
		fprintf(stderr, "printContent(int fd). fd < 0!\n");
		exit(ERROR_VAL);
	} // incorrect usage

	if (lseek(fd, 0, SEEK_SET) == (off_t) -1) {
		return -1;
	}

	char buff[BUFF_SIZE + 1];
	ssize_t n;

	while ((n = my_read(fd, buff, BUFF_SIZE)) > 0) {
		buff[n] = '\0';
		printf("%s", buff);
	}

	if (n == -1) {
		return -2;
	}

	return 0;
}