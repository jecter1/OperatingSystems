#include "table.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define INIT_TABLE_SIZE 1024
#define BUFF_SIZE 1024
#define ERROR_VAL -1

Table* makeTable(int fd) {
	if (fd < 0) {
		fprintf(stderr, "makeTable(int fd). fd < 0!\n");
		exit(ERROR_VAL);
	} // incorrect usage

	// allocate memory for table
	int memSize = INIT_TABLE_SIZE;
	off_t* length = (off_t*) malloc(sizeof(off_t) * memSize);
	if (length == NULL) {
		return NULL;
	}
	off_t* offset = (off_t*) malloc(sizeof(off_t) * memSize);
	if (offset == NULL) {
		free(length);
		return NULL;
	}

	length[0] = 0;
	offset[0] = 0;

	int lineIndex = 0;
	char buff[BUFF_SIZE];
	ssize_t n;

	// reading file. If there are '\n' -> make new table entry
	while ((n = read(fd, buff, BUFF_SIZE)) > 0) {
		for (ssize_t i = 0; i < n; i++) {
			if (buff[i] == '\n') {
				// realloc if there are no space for new line
				if (memSize == lineIndex + 1) { 
					memSize *= 2;
					length = (off_t*) realloc(length, sizeof(off_t) * memSize);
					if (length == NULL) {
						free(length);
						free(offset);
						return NULL;
					}
					offset = (off_t*) realloc(offset, sizeof(off_t) * memSize);
					if (offset == NULL) {
						free(length);
						free(offset);
						return NULL;
					}
				}
				lineIndex++;
				length[lineIndex] = 0;
				offset[lineIndex] = offset[lineIndex - 1] + length[lineIndex - 1] + 1;
			} else {
				length[lineIndex]++;
			}
		}
	}
	if (n == -1) {
		free(length);
		free(offset);
		return NULL;
	} // read error

	// transfer table (arrays of lengths/offsets and size) to struct Table
	Table* result = (Table*) malloc(sizeof(Table));
	if (result == NULL) {
		free(length);
		free(offset);
		return NULL;
	}
	result->length = length;
	result->offset = offset;
	result->size = lineIndex + 1;

	return result;
}

int printLine(int fd, Table* table, int num) {
	if (table == NULL) {
		fprintf(stderr, "printLine(int fd, Table* table, int num). table is NULL!\n");
		exit(ERROR_VAL);
	} // incorrect usage
	if (fd < 0) {
		fprintf(stderr, "printLine(int fd, Table* table, int num). fd < 0!\n");
		exit(ERROR_VAL);
	} // incorrect usage

	if (num > table->size || num < 1) {
		return -1;
	} // out of range

	size_t buffSize = table->length[num - 1];
	char buff[buffSize + 1];
	
	if (lseek(fd, table->offset[num - 1], SEEK_SET) == (off_t) -1) {
		return -2;
	}
	
	if (read(fd, buff, buffSize) < 0) {
		return -2;
	}

	buff[buffSize] = '\0';
	printf("%s\n", buff);
	
	return 0;
}