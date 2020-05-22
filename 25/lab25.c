#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>

#define BUFSIZE 20

void print_wstatus(char* pname, int wstatus);

pid_t wait_while_EINTR(int* wstatus);

ssize_t read_while_EINTR(int fd, void* buf, size_t count);

ssize_t write_while_EINTR(int fd, const void* buf, size_t count);

int main() {
	pid_t cpid1, cpid2, w1, w2;
	int wstatus1, wstatus2;
	int pipefd[2];

	if (pipe(pipefd) == -1) {
		perror("pipe");
		exit(EXIT_FAILURE);
	}

	if ((cpid1 = fork()) > 0 && (cpid2 = fork()) > 0) { // Parent
		close(pipefd[0]);
		close(pipefd[1]);
		printf("Parent: waiting for children\n");
		w1 = wait_while_EINTR(&wstatus1);
		w2 = wait_while_EINTR(&wstatus2);
		if (w1 == -1 || w2 == -1) {
			perror("wait");
			exit(EXIT_FAILURE);
		}
		if (w1 == cpid1 && w2 == cpid2) {
			print_wstatus("child1", wstatus1);
			print_wstatus("child2", wstatus2);
		} else if (w1 == cpid2 && w2 == cpid1) {
			print_wstatus("child1", wstatus2);
			print_wstatus("child2", wstatus1);
		}
		exit(EXIT_SUCCESS);
	} else if (cpid1 == 0) { // Child1
		close(pipefd[0]);
		char buf[BUFSIZE] = "HeLlO fRoM cHiLd1";
		if (write_while_EINTR(pipefd[1], buf, strlen(buf)) < 0) {
			perror("write");
			exit(EXIT_FAILURE);
		}
		exit(EXIT_SUCCESS);
	} else if (cpid2 == 0) { // Child2
		close(pipefd[1]);
		char buf[BUFSIZE] = "";
		if (read_while_EINTR(pipefd[0], buf, BUFSIZE) < 0) {
			perror("read");
			exit(EXIT_FAILURE);
		}
		for (int i = 0; i < strlen(buf); ++i) {
			buf[i] = toupper((unsigned char) buf[i]);
		}
		printf("Child2 recieved from child1 message: %s\n", buf);
		exit(EXIT_SUCCESS);
	} else { // Error
		perror("fork");
		exit(EXIT_FAILURE);
	}
}

void print_wstatus(char* pname, int wstatus) {
	if (pname == NULL) {
		fprintf(stderr, "printStatus(char* pname, int wstatus)\n");
		fprintf(stderr, "pname == NULL\n");
		exit(EXIT_FAILURE);
	} // incorrect usage

	if (WIFEXITED(wstatus)) {
		printf("Parent: %s was exited with exit code %d\n", pname, WEXITSTATUS(wstatus));
	} else if (WIFSIGNALED(wstatus)) {
		printf("Parent: %s was killed by signal %d\n", pname, WTERMSIG(wstatus));
	}
}

pid_t wait_while_EINTR(int* wstatus) {
	pid_t w;
	do { w = wait(wstatus); } while (w == -1 && errno == EINTR);
	return w;
}

ssize_t read_while_EINTR(int fd, void* buf, size_t count) {
	ssize_t n;
	do { n = read(fd, buf, count); } while (n == -1 && errno == EINTR);
	return n;
}

ssize_t write_while_EINTR(int fd, const void* buf, size_t count) {
	ssize_t n;
	do { n = write(fd, buf, count); } while (n == -1 && errno == EINTR);
	return n;
}
