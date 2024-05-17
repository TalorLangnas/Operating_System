#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <signal.h>
#include <asm-generic/socket.h>

#define PORT 8080
#define MAX_CONNECTIONS 20
#define MAX_CLIENTS 100
#define BUFFER_SZ 2048

/* Client structure */
typedef struct{
	struct sockaddr_in address;
	int sockfd;
	int uid;
	char name[32];
} Client;

// Client initialize function
Client* client_create(int sockfd, struct sockaddr_in address, int uid);

// handel function
void *handle_event(void *arg);

/*Server struct*/
typedef struct {
	char *ip;
	int port;
	int option;
	int listenfd;
	int connfd;
    struct sockaddr_in serv_addr;
	Client *clients[MAX_CLIENTS];
	// int number_of_clients;
	_Atomic unsigned int number_of_clients;
	pthread_mutex_t clients_mutex;
} Server;

// Server initialize function
Server* server_create(char *ip, int port, int option);
// Add clients to queue 
void arr_add(Client *cl, Server *server);
// Remove clients from array 
void arr_remove(int uid, Server *server);

void eventLoop(Server *server);


/*Event structure*/
typedef struct {
    Client *client;
	Server *server;
} Event;

// Event initialize function
Event* event_create(Client *client, Server *server);

void send_message(char *s, int uid, Server *server);
