#ifndef TABLE_H
#define TABLE_H

#include <sys/types.h>

typedef struct {
	off_t* length;
	off_t* offset;
	int size;
} Table;

Table* makeTable(int fd);
int printLine(int fd, Table* table, int num);

#endif