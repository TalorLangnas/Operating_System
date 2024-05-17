#include "proactor.h"

// Client initialize function
Client* client_create(int sockfd, struct sockaddr_in address, int uid){
    Client *client = (Client*)malloc(sizeof(Client));
    client->sockfd = sockfd;
    client->address = address;
    client->uid = uid + 1;
    return client;
}
// Server initialize function
Server* server_create(char *ip, int port, int option){
    Server *server = (Server*)malloc(sizeof(Server));
    server->ip = ip;
    server->port = port;
    server->option = option;
    server->listenfd = 0;
    server->connfd = 0;
    server->serv_addr.sin_family = AF_INET;
    server->serv_addr.sin_addr.s_addr = inet_addr(ip);
    server->serv_addr.sin_port = htons(port);
    pthread_mutex_init(&server->clients_mutex, NULL);
    server->number_of_clients = 0;
    return server;
};

// Event initialize function
Event* event_create(Client *client, Server *server)
{
    Event* event = (Event*)malloc(sizeof(Event));
    event->client = client;
    event->server = server;

    return event;
}


/* Handle all communication with the Client */
void *handle_event(void *arg)
{
    char buff_out[BUFFER_SZ];
	char name[32];
	int leave_flag = 0;

    // initialize the event
    Event *event = (Event *)arg;
    
    // extract the client from the event
	Client *cli = (Client *)event->client;
    // extract the server from the event
    Server *server = (Server *)event->server;

    // Name
	if(recv(cli->sockfd, name, 32, 0) <= 0 || strlen(name) <  2 || strlen(name) >= 32-1){
		printf("Didn't enter the name.\n");
		leave_flag = 1;
	} else{
		strcpy(cli->name, name);
		sprintf(buff_out, "%s has joined\n", cli->name);
		printf("%s", buff_out);
		send_message(buff_out, cli->uid, server);
	}

    bzero(buff_out, BUFFER_SZ);

    while(1){
		if (leave_flag) {
			break;
		}

		int receive = recv(cli->sockfd, buff_out, BUFFER_SZ, 0);
		if (receive > 0){
			if(strlen(buff_out) > 0){
				send_message(buff_out, cli->uid, server);

				// str_trim_lf(buff_out, strlen(buff_out));
				printf("%s -> %s\n", buff_out, cli->name);
			}
		} else if (receive == 0 || strcmp(buff_out, "exit") == 0){
			sprintf(buff_out, "%s has left\n", cli->name);
			printf("%s", buff_out);
			send_message(buff_out, cli->uid, server);
			leave_flag = 1;
		} else {
			printf("ERROR: -1\n");
			leave_flag = 1;
		}

		bzero(buff_out, BUFFER_SZ);
	}

  /* Delete client from queue and yield thread */
	close(cli->sockfd);
  arr_remove(cli->uid, server);
  free(cli);
  server->number_of_clients--;
  pthread_detach(pthread_self());

	return NULL;
}

void eventLoop(Server *server)
{
    struct sockaddr_in cli_addr;
    pthread_t tid;
    while(1){
        socklen_t clilen = sizeof(cli_addr);
		server->connfd = accept(server->listenfd, (struct sockaddr*)&cli_addr, &clilen);

		// Check if max clients is full
		if((server->number_of_clients + 1) == MAX_CLIENTS){
			printf("Max clients reached. Rejected: ");
			// print_client_addr(cli_addr);
			printf(":%d\n", cli_addr.sin_port);
			close(server->connfd);
			continue;
		}

        // Define client details
        Client *cli = client_create(server->connfd, cli_addr, server->number_of_clients);
        server->number_of_clients++;

        // Add client to the queue and fork thread 
		arr_add(cli, server);
        // create event
        Event *event = event_create(cli, server);

		pthread_create(&tid, NULL, &handle_event, (void*)event);

    }
};

/* Add clients to queue */
void arr_add(Client *cl, Server *server){
	pthread_mutex_lock(&server->clients_mutex);

	for(int i=0; i < MAX_CLIENTS; ++i){
		if(!server->clients[i]){
			server->clients[i] = cl;
			break;
		}
	}

	pthread_mutex_unlock(&server->clients_mutex);
}

// Remove clients from array 
void arr_remove(int uid, Server *server){
	pthread_mutex_lock(&server->clients_mutex);

	for(int i=0; i < MAX_CLIENTS; ++i){
		if(server->clients[i]){
			if(server->clients[i]->uid == uid){
				server->clients[i] = NULL;
				break;
			}
		}
	}

	pthread_mutex_unlock(&server->clients_mutex);
}

/* Send message to all clients except sender */
void send_message(char *s, int uid, Server *server){
	pthread_mutex_lock(&server->clients_mutex);

	for(int i=0; i<MAX_CLIENTS; ++i){
		if(server->clients[i]){
			if(server->clients[i]->uid != uid){
				if(write(server->clients[i]->sockfd, s, strlen(s)) < 0){
					perror("ERROR: write to descriptor failed");
					break;
				}
			}
		}
	}

	pthread_mutex_unlock(&server->clients_mutex);
}


    
