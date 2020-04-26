#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <pthread.h>
#include <string.h>
#include <time.h>

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"
#define RESET "\033[0m"

typedef unsigned char byte;

struct client_args {
	int socket;
	char name[16];
	int clientIndex;
};

struct clientMessage {
	char msg[5000];
	char client[16];
	time_t timeSent;
};

char *trim(char *str)
{
    size_t len = 0;
    char *frontp = str;
    char *endp = NULL;

    if( str == NULL ) { return NULL; }
    if( str[0] == '\0' ) { return str; }

    len = strlen(str);
    endp = str + len;

    /* Move the front and back pointers to address the first non-whitespace
     * characters from each end.
     */
    while( isspace((unsigned char) *frontp) ) { ++frontp; }
    if( endp != frontp )
    {
        while( isspace((unsigned char) *(--endp)) && endp != frontp ) {}
    }

    if( frontp != str && endp == frontp )
            *str = '\0';
    else if( str + len - 1 != endp )
            *(endp + 1) = '\0';

    /* Shift the string so that it starts at str so that if it's dynamically
     * allocated, we can still free it on the returned pointer.  Note the reuse
     * of endp to mean the front of the string buffer now.
     */
    endp = str;
    if( frontp != str )
    {
            while( *frontp ) { *endp++ = *frontp++; }
            *endp = '\0';
    }

    return str;
}

void *messageReceiveHandler(void *args) {
	int server = (int)args;
	while(1) {
		byte buffer[6000];
		memset(buffer, 0, 6000);
		recv(server, buffer, 6000, 0);
		printMessage(buffer);
	}
}

void printMessage(byte *message) {
	time_t timeSent = time(NULL);
	char name[50], msg[6000];
	
	memset(name, 0, 50);
	memset(msg, 0, 6000);

	memcpy(name, message, 50);
	memcpy(msg, message+50, strlen(message+50));
	
	struct tm *ptm = localtime(&timeSent);
	printf("[\x1B[34m%02d:%02d:%02d \x1B[33m(%s)\033[0m]: \x1B[36m%s\033[0m\n", ptm->tm_hour, ptm->tm_min, ptm->tm_sec, trim(name), trim(msg));
}

int main(int argc, char **argv) {
	pthread_t thread;
	int serverSocket, acceptSocket;
	struct sockaddr_in bindAddress;
	struct sockaddr connectionAddress_t;

	serverSocket = socket(AF_INET, SOCK_STREAM, 0);

	bindAddress.sin_family = AF_INET;
	bindAddress.sin_port = htons(1234);
	inet_pton(AF_INET, argv[1], &bindAddress.sin_addr.s_addr);

	connect(serverSocket, (struct sockaddr *)&bindAddress, sizeof(bindAddress));
	send(serverSocket, argv[2], strlen(argv[2]), 0);

	pthread_create (&thread, NULL, messageReceiveHandler, (void *)serverSocket);
	while(1) {
		char message[5000];
		byte buffer[6000];
		memset(buffer, 0, 6000);
		memset(message, 0, 6000);

		gets(message);

		if(strcmp(message, "/quit") == 0) break;

		memcpy(buffer, argv[2], strlen(argv[2]));
		int i;
		for(i=0;i<50-strlen(argv[2]);i++)
			strcat(buffer, " ");
		strcat(buffer, message);
		printMessage(buffer);

		if(strlen(message) == 0) continue;

		send(serverSocket, buffer, strlen(buffer), 0);
	}	
	pthread_exit(thread);
	close(serverSocket);
	return 0;
}
