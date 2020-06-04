#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define BUFSIZE 256

extern char *tzname[];

int main() {
	time_t now;
	int err;

	if (putenv("TZ=America/Los_Angeles") != 0) {
		perror("putenv()");
		exit(EXIT_FAILURE);
	}

	if (time(&now) == (time_t)(-1)) {
		perror("time()");
		exit(EXIT_FAILURE);
	}

	// ctime_r()
	char timeStr[BUFSIZE];
	if (ctime_r(&now, timeStr) == NULL) {
		perror("ctime_r()");
		exit(EXIT_FAILURE);
	}

	err = printf(">ctime_r():\n%s", timeStr);
	if (err < 0) {
		perror("Writing time (ctime_r)");
		exit(EXIT_FAILURE);
	}
	// ~ctime_r()

	// localtime_r()
	struct tm result;
	if (localtime_r(&now, &result) == NULL) {
		perror("localtime_r()");
		exit(EXIT_FAILURE);
	}

	err = printf(">localtime_r():\n%d/%d/%02d %d:%02d %s\n",
		result.tm_mon + 1,
		result.tm_mday,
		result.tm_year % 100,
		result.tm_hour,
		result.tm_min,
		tzname[result.tm_isdst]);

	if (err < 0) {
		perror("Writing time (localtime_r)");
		exit(EXIT_FAILURE);
	}
	// ~localtime_r()

	exit(EXIT_SUCCESS);
}