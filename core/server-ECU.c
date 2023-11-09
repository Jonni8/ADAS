#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/unistd.h>

#define MAX_CLIENTS 2
#define SOCKET_NAME "ADAS"
#define BUFFER_SIZE 2048

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
    int front, steer, rec; //components
    socklen_t address_size;
    struct sockaddr_un serverAddress, clientAddress, new_addr;
    struct sockaddr* serverPtr;
    int client_sockets[MAX_CLIENTS];
    
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
    
    char buffer[BUFFER_SIZE] = {0};
    char line[2048];
    int buffer_length = 0;
    int bytes_received;

    front = 0; 
    steer = 1;

    while(1) {
        for (int i = 0; i < MAX_CLIENTS; i++) {
            new_socket = accept(server_fd, (struct sockaddr *)&new_addr, &address_size);
            client_sockets[i] = new_socket;
            printf("Client %d connected\n", i + 1);
        }

        // write(client_sockets[front], buffer, sizeof(buffer));
        // printf("Sent message to Front\n");

        while((recv(client_sockets[front], buffer, sizeof(buffer), 0)) > 0) {
            printf("Buffer: %s\n", buffer);
            if(strcmp(buffer, "SINISTRA") || strcmp(buffer, "DESTRA") ) {  

                write(client_sockets[steer], buffer, strlen(buffer));
                printf("Sent message to Steer\n");
                //write(client_sockets[front], buffer, sizeof(buffer));
            }
        }

        // while(( bytes_received = recv(client_sockets[front], buffer, sizeof(buffer), 0)) > 0) {
        //     if (buffer_length + bytes_received < BUFFER_SIZE) {
        //         printf("Buffer: %s\n", buffer);
        //         memcpy ( buffer + buffer_length, line, bytes_received);
        //         buffer_length += bytes_received;
        //     }
        // }
        // close(client_sockets[front]);
        // for(int i = 0; i < buffer_length; i++) {
        //     putchar(buffer[i]);
        // }

        // Send the message to the receiver client
        // if(strcmp(buffer,"DESTRA") || strcmp(buffer,"SINISTRA")) {
        //     send(client_sockets[steer], buffer, strlen(buffer), 0);
        // }

        //send(client_sockets[rec], buffer, strlen(buffer), 0);
    }
    close(server_fd);

    return 0;
}
