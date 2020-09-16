#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>


char message[1024 * 256];
char block[1024 * 256];

const char datafile[]="datafile";

int main(int ac, char **av)
{
	int sock, sock2;
	struct sockaddr_in sinme;
	struct sockaddr_in from;
	socklen_t len=sizeof(from);
	short port=9000;
	int blockLen;
	FILE *fp;

        if (ac > 1) {
                if (isdigit(*av[1])) {
                        port = atoi(av[1]);
                }
                else {
                        printf("Usage: %s [port-no]\n", av[0]);
                        return 0;
                }
        }
        printf("Server: listen port=%d.\n", port);

	sinme.sin_family = PF_INET;
	sinme.sin_port = htons(port);
	sinme.sin_addr.s_addr = htonl(INADDR_ANY);

	sock = socket(PF_INET, SOCK_STREAM, 0);

	if(bind(sock, (struct sockaddr *)&sinme, sizeof(sinme)) < 0) {
		printf("Server: Can't bind.\n");
		exit(0);
	}
	listen(sock, SOMAXCONN);

	printf("Waiting for Connection Request.\n");
	sock2 = accept(sock, (struct sockaddr *)&from, &len);
	if (sock2 < 0) {
		printf("Server: Can't accepted.\n");
		exit(0);
	}
	else {
		printf("Server: Connected from %s.\n", inet_ntoa(from.sin_addr));
	}
	
	while(1) {
		fp = fopen(datafile, "rb");
		if (fp == NULL) {
			printf("Server: Can't open file:%s\n", datafile);
			exit(0);
		}
		blockLen = fread(message, 1, 1024 * 256, fp);
		fclose(fp);
		sendto(sock2, message, blockLen, 0, (struct sockaddr *) &from, len);
		printf("Server: Press Enter key.");
		fgets(message, 1, stdin);
	}
	close(sock);
}
