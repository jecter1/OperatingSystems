#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
	char *data;
	struct node *next;
} Node;

typedef struct list {
	Node *head;
	Node *tail;
} List;

List *createList() {
	List *L = (List *)malloc(sizeof(List));
	if (L == NULL) {
		perror("Creating List (memory allocation)");
		exit(1);
	}

	L->head = NULL;
	L->tail = NULL;
	return L;
}

void push(List *L, char *str) {
	if (L == NULL) {
		return;
	}

	Node *new = (Node *)malloc(sizeof(Node));
	if (new == NULL) {
		perror("Pushing node (memory allocation)");
		exit(1);
	}

	new->data = strdup(str);
	if (new->data == NULL) {
		perror("Pushing node (copying string");
		exit(1);
	}

	new->next = NULL;

	if (L->head == NULL) {
		L->head = new;
		L->tail = new;
	} else {
		L->tail->next = new;
		L->tail = L->tail->next;
	}
}

void printList(List *L) {
	if (L == NULL || L->head == NULL) {
		return;
	}

	Node *tmp = L->head;
	while (tmp != NULL) {
		if (printf(">%s", tmp->data) < 0) {
			perror("Printing string");
			exit(1);
		}
		tmp = tmp->next;
	}
}

void removeList(List *L) {
	if (L == NULL || L->head == NULL) {
		return;
	}

	Node * tmp;

	while (L->head != NULL) {
		tmp = L->head;
		L->head = L->head->next;
		free(tmp->data);
		free(tmp);
	}
	free(L);
}

int main() {
	List *L = createList();

	char buf[BUFSIZ];
	if (fgets(buf, BUFSIZ, stdin) == NULL) {
		perror("Getting new line");
		exit(1);
	}

	char *str;

	while (buf[0] != '.') {
		str = strdup(buf);
		if (str == NULL) {
			perror("Copying string from buffer");
			exit(1);
		}

		push(L, str);
		
		if (fgets(buf, BUFSIZ, stdin) == NULL) {
			perror("Getting new line");
			exit(1);
		}
		
		free(str);
	}

	printList(L);
	removeList(L);

	exit(0);
}