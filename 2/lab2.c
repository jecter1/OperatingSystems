#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

extern char *tzname[];

main() {
	time_t now;
	struct tm *sp;
	int err;
	char *timeStr;

	if (putenv("TZ=America/Los_Angeles") != 0) {
		perror("Setting TZ");
		exit(1);
	}

	if (time(&now) == (time_t)(-1)) {
		perror("Getting current time");
		exit(1);
	}

	timeStr = ctime(&now);
	if (timeStr == NULL) {
		perror("Making string (ctime)");
		exit(1);
	}

	err = printf("%s", timeStr);
	if (err < 0) {
		perror("Writing time (ctime)");
		exit(1);
	}

	sp = localtime(&now);
	if (sp == NULL) {
		perror("Getting local time");
		exit(1);
	}

	err = printf("%d/%d/%02d %d:%02d %s\n",
		sp->tm_mon + 1,
		sp->tm_mday,
		sp->tm_year % 100,
		sp->tm_hour,
		sp->tm_min,
		tzname[sp->tm_isdst]);

	if (err < 0) {
		perror("Writing time (localtime)");
		exit(1);
	}

	exit(0);
}