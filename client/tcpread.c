#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#define _DEBUG (1)

#define MAX_TIME 5
#define TRUE  1
#define FALSE 0

const char *defaultIPaddress = "127.0.0.1";
const short defaultPort = 9000;

double my_clock(void)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec + (double)tv.tv_usec * 1e-6;
}

int main(int argc, char *argv[])
{
	int sock;
	struct sockaddr_in sinhim;
	short port;
	char ipAddress[256] = "";
	char fileName[256] = "";
	struct hostent *host = NULL;
	int i;
	int iCount;
	char message[1024];
	int startFlag = FALSE;
	double dStart;
	int len;
	int text_len;

	strcpy(ipAddress, defaultIPaddress);
	port = defaultPort;
	for (i = 1; i < argc; i += 2) {
		if (!strcmp(argv[i], "-f")) {
			if (i + 1 < argc) {
				strcpy(fileName, argv[i + 1]);
			}
			else {
				printf("file name not found.\n"); 
				exit(1);
			}
		}
		else if (!strcmp(argv[i], "-i")) {
			if (i + 1 < argc) {
				strcpy(ipAddress, argv[i + 1]);
			}
			else {
				printf("ip address not found.\n"); 
				exit(1);
			}
		}
		else if (!strcmp(argv[i], "-h")) {
			if (i + 1 < argc) {
				host = gethostbyname(argv[i + 1]);
				if (host == NULL) {
					printf("host name invalid.\n"); 
					exit(1);
				}
			}
			else {
				printf("host name not found.\n"); 
				exit(1);
			}
		}
		else if (!strcmp(argv[i], "-p")) {
			if (i + 1 < argc) {
				port = atoi(argv[i + 1]);
			}
			else {
				printf("port not found.\n"); 
				exit(1);
			}
		}
		else {
			printf("parameter invalid.\n");
			exit(1);
		}
	}

	if (host != NULL) {
		for(iCount = 0 ; host->h_addr_list[iCount] ; iCount++) {
			sprintf(ipAddress, "%d.%d.%d.%d" ,
				(unsigned char)*((host->h_addr_list[iCount])) ,
				(unsigned char)*((host->h_addr_list[iCount]) + 1) ,
				(unsigned char)*((host->h_addr_list[iCount]) + 2) ,
				(unsigned char)*((host->h_addr_list[iCount]) + 3)
			);
		}
	}
#ifdef _DEBUG
	printf("IP:%s\n", ipAddress);
	printf("PORT:%d\n", port);
	printf("FILE:%s\n", fileName);
#endif
	sinhim.sin_family = PF_INET;
	sinhim.sin_port = htons(port);
	sinhim.sin_addr.s_addr = inet_addr(ipAddress);

	sock = socket(PF_INET, SOCK_STREAM, 0);

	if (connect(sock, (struct sockaddr *) &sinhim, sizeof(sinhim) ) < 0) {
		printf("Can't connect.\n");
		exit(1);
	}

	startFlag = FALSE;
	while (1) {
		len = recv(sock, message, 1024,  0);
		if (len == 0) {
			printf("Recv Length == 0\n");
			break;
		}

		text_len = strlen(message);
		printf("recv_len=%d, text_len=%d\n", len, text_len);  

		if (text_len < len) {
			len = text_len;
		}

		printf("<%s>\n", message);
		dStart = my_clock();

		if (my_clock() - dStart > MAX_TIME && startFlag) {
			startFlag = FALSE;
#ifdef _DEBUG
			printf("time out.\n");
#endif
		}

	}
	close(sock);

	return 0;
}
