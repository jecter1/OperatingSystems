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
		return NULL;
	}

	L->head = NULL;
	L->tail = NULL;
	return L;
}

int insert(List *L, char *str) {
	if (L == NULL) {
		perror("Insert. List is NULL");
		exit(1);
	}

	Node *new = (Node *)malloc(sizeof(Node));
	if (new == NULL) {
		return 1;
	}

	new->data = strdup(str);
	if (new->data == NULL) {
		return 1;
	}

	new->next = NULL;

	if (L->head == NULL) {
		L->head = new;
		L->tail = new;
	} else {
		L->tail->next = new;
		L->tail = L->tail->next;
	}

	return 0;
}

int printList(List *L) {
	if (L == NULL) {
		perror("printList(List *L). L is NULL");
		exit(1);
	}

	Node *tmp = L->head;
	while (tmp != NULL) {
		if (printf(">%s", tmp->data) < 0) {
			return 1;
		}
		tmp = tmp->next;
	}

	return 0;
}

void removeList(List *L) {
	if (L == NULL) {
		perror("removeList(List *L). L is NULL");
		exit(1);
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
	if (L == NULL) {
		perror("Creating List");
		exit(1);
	}

	char buf[BUFSIZ];

	if (fgets(buf, BUFSIZ, stdin) == NULL) {
		removeList(L);
		perror("Getting new line");
		exit(1);
	}

	while (buf[0] != '.') {
		if (insert(L, buf) != 0) {
			removeList(L);
			perror("Pushing line");
			exit(1);
		}

		if (fgets(buf, BUFSIZ, stdin) == NULL) {
			removeList(L);
			perror("Getting new line");
			exit(1);
		}
	}

	if (printList(L) != 0) {
		removeList(L);
		perror("Printing List");
		exit(1);
	}
	removeList(L);

	exit(0);
}
