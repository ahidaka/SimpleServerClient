#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>

#define DAFAULT_SIZE (256 * 1024)

void usage(char *myname)
{
	printf("Usage %s [finename [filesize]]\n", myname);
}

int main(int ac, char **av)
{
	char *name = "datafile";
	int length, i, block;
	char buffer[256];
	FILE *f;

	length = DAFAULT_SIZE;
	if (ac > 1) {
		if (*av[1] == '-') {
			usage(av[0]);
			exit (0);
		}
		name = av[1];
	}
	if (ac > 2) {
		if (isdigit(*av[2])) {
			length = atoi(av[2]);
		}
		else {
			usage(av[0]);
		}
	}

	printf("file=%s size=%d\n", name, length);
	
	for(i = 0; i < 256; i++) {
		buffer[i] = i % 256;
	}

	f = fopen(name, "w");
	if (f == NULL) {
		printf("Cannot create file=%s\n", name);
		exit(1);
	}
	block = 1; //length / 256;
	for(i = 0; i < (length / 256); i++) {
		printf("%d:%d\n", i, length /256);
		fwrite(buffer, 256, block, f);
	}
	fclose(f);
	
	return 0;
}
