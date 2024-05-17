#include "proactor.h"

int main()
{
    Server *server;

    server = server_create("127.0.0.1", PORT, 1);

    // Define the socket
    server->listenfd = socket(AF_INET, SOCK_STREAM, 0);

	if(setsockopt(server->listenfd, SOL_SOCKET,(SO_REUSEPORT | SO_REUSEADDR),(char*)&server->option,sizeof(server->option)) < 0){
		perror("ERROR: setsockopt failed");
    return EXIT_FAILURE;
	}

    // Bind
    if(bind(server->listenfd, (struct sockaddr*)&server->serv_addr, sizeof(server->serv_addr)) < 0) {
    perror("ERROR: Socket binding failed");
    return EXIT_FAILURE;
    }

       // Listen
    if (listen(server->listenfd, MAX_CONNECTIONS) < 0) {
    perror("ERROR: Socket listening failed");
    return EXIT_FAILURE;
	}

    printf("=== WELCOME TO THE CHATROOM ===\n");

    eventLoop(server);

    return 0;
}