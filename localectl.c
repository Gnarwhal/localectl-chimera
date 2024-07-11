#include <stdio.h>
#include "org.freedesktop.locale1.h"

#define PROJECT_NAME "localectl"

int main(int argc, char **argv) {
	if(argc != 1) {
		printf("%s takes no arguments.\n", argv[0]);
		return 1;
	}
	printf("This is project %s.\n", PROJECT_NAME);
	return 0;
}
