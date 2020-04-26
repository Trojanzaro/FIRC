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

int clients[5];
int c_c = 0;

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

void *connectionHandler(void *args) {
	struct client_args *clientArguments = (struct client_args *)args;

	byte newConn[100] = "SERVER                                                  \x1B[32mUser: (\x1B[33m";
	byte buffer[6000];
	recv(clientArguments->socket, buffer, 100, 0); 
	strcat(newConn, buffer);
	strcat(newConn, "\x1B[32m) Connected!\033[0m");
	broadcastMessage(clientArguments->socket, newConn);

	while(1) {
		memset(buffer, 0, 6000);
		if(recv(clientArguments->socket, buffer, 6000, 0) < 0);
		printf("Message received! Broadcasting . . .\n");
		printf("[[%s]]\n",buffer);
		broadcastMessage(clientArguments->socket, buffer);
	}
	printf("HE LEFT!!!!\n");
	clientDisconnect(clientArguments->socket, clientArguments->clientIndex);
	pthread_exit(NULL);
}

void broadcastMessage(int sock, byte *buffer) {
	int i;
	for(i=0; i<5; i++)
		if(clients[i] == sock) continue;
		else send(clients[i], buffer, strlen(buffer), 0);
		
}

void clientDisconnect(int sock, int index) {
	c_c--;
	close(clients[index]);
}

int main() {
	int serverSocket, acceptSocket;
	struct sockaddr_in bindAddress;
	struct sockaddr connectionAddress_t;
	socklen_t *connectioAddress_l = (socklen_t *)malloc(sizeof(socklen_t));

	serverSocket = socket(AF_INET, SOCK_STREAM, 0);

	bindAddress.sin_family = AF_INET;
	bindAddress.sin_port = htons(1234);
	bindAddress.sin_addr.s_addr = 0x00000000;

	bind(serverSocket, (struct sockaddr *)&bindAddress, sizeof(bindAddress));
	listen(serverSocket, 2);
	
	printf("ServerSetup on port 1234\n");
	pthread_t threads[5];
	while(1) {
		c_c++;
		acceptSocket = accept(serverSocket, &connectionAddress_t, connectioAddress_l);
		struct sockaddr_in *connectionAddress = (struct sockaddr_in *)&connectionAddress_t;
		char sConnecteAddress[16];
		inet_ntop(AF_INET, (void *)&connectionAddress->sin_addr.s_addr, sConnecteAddress, 16);
		printf("Connection from: %s\n", sConnecteAddress);
		struct client_args *cArgs = (struct client_args *)malloc(sizeof(struct client_args));
		strcpy(cArgs->name, sConnecteAddress);
		cArgs->socket = acceptSocket;
		cArgs->clientIndex = c_c;
		int rc = pthread_create (&threads[c_c], NULL, connectionHandler, (void *)cArgs);
		clients[c_c] = acceptSocket;
	}
	return 0;
}
