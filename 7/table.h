#ifndef TABLE_H
#define TABLE_H

#include <sys/types.h>

typedef struct {
	off_t* length;
	off_t* offset;
	int size;
} Table;

Table* makeTable(char* mappedFile, off_t fileSize);
int printLine(char* mappedFile, Table* table, int num);

#endif