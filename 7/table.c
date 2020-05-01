#include "table.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#define INIT_TABLE_SIZE 1024
#define ERROR_VAL -1
#define STDOUT_FD 1

Table* makeTable(char* mappedFile, off_t fileSize) {
	if (mappedFile == NULL) {
		fprintf(stderr, "makeTable(char* mappedFile, off_t fileSize). mappedFile == NULL!\n");
		exit(ERROR_VAL);
	} // incorrect usage
	if (fileSize < 0) {
		fprintf(stderr, "makeTable(char* mappedFile, off_t fileSize). fileSize < 0!\n");
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

	// pass through a file. If there are '\n' -> make new table entry
	for (off_t i = 0; i < fileSize; ++i) {
		if (mappedFile[i] == '\n') {
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

int printLine(char* mappedFile, Table* table, int num) {
	if (mappedFile == NULL) {
		fprintf(stderr, "printLine(char* mappedFile, Table* table, int num). mappedFile == NULL!\n");
		exit(ERROR_VAL);
	} // incorrect usage
	if (table == NULL) {
		fprintf(stderr, "printLine(char* mappedFile, Table* table, int num). table == NULL!\n");
		exit(ERROR_VAL);
	} // incorrect usage

	if (num > table->size || num < 1) {
		return -1;
	} // out of range
	
	off_t offset = table->offset[num - 1];
	off_t length = table->length[num - 1];

	ssize_t n;
	do {
		n = write(STDOUT_FD, mappedFile + offset, length);
	} while (n == -1 && errno == EINTR);

	if (n == -1) {
		return -2;
	}
	printf("\n");

	return 0;
}