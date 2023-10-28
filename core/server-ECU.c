#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/unistd.h>

#define MAX_CLIENTS 5
#define SOCKET_NAME "ADAS"

int readstring(int fd, char *bufferData) {
    int n; 
    do {
        n = read (fd, bufferData, 1);
    } while ( n > 0 && *bufferData++ != '\0' );
    return( n > 0); 
}

int readData(int fd, char *bufferData, char *destbufferData, int lentgh) {
    
    while(readstring( fd, bufferData)) {
        for(int i = 0; i < lentgh; i++) {
            *destbufferData++ = *bufferData++;
        }
    }
    printf("%s\n",bufferData);
}

int main(int argc, char const *argv[]) {
    int server_fd, client_fd, new_socket;
    int sender, receiver, rec; //
    socklen_t address_size;
    struct sockaddr_un serverAddress, clientAddress, new_addr;
    struct sockaddr* serverPtr;
    int client_sockets[MAX_CLIENTS];
    char buffer[256];
    
    serverPtr = (struct sockaddr*) &serverAddress; 
    unsigned int serverLen = sizeof(serverAddress); 
    unsigned int clientLen = sizeof(clientAddress); 

    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);  
    if (server_fd == -1) {
        printf("Could not create socket server");
        exit(1);
    }

    serverAddress.sun_family = AF_UNIX;                 
    strcpy(serverAddress.sun_path, SOCKET_NAME);     
    unlink(SOCKET_NAME); 

    bind(server_fd, serverPtr, serverLen);                 
    if (listen(server_fd, MAX_CLIENTS) == 0) {
        printf("Listening...\n");
    } else {
        printf("Error in Listening...\n");
    }          

    address_size = sizeof(new_addr);

    while(1) {
        for (int i = 0; i < MAX_CLIENTS; i++) {
            new_socket = accept(server_fd, (struct sockaddr *)&new_addr, &address_size);
            client_sockets[i] = new_socket;
            printf("Client %d connected\n", i + 1);
        }

        recv(client_sockets[sender], buffer, sizeof(buffer), 0);
        printf("Received from Client %d: %s\n", sender + 1, buffer);

        // Send the message to the receiver client
        send(client_sockets[receiver], buffer, strlen(buffer), 0);

        send(client_sockets[rec], buffer, strlen(buffer), 0);
    }

    return 0;
}
