#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/unistd.h>

#include "utility.h"

#define MAX_CLIENTS 1
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

int getInput(int fd) {
    char command[100];
    if(readMessage(fd, command) == 0) {
        printf("C: %s\n", command);
       if (strcmp(command, "ARRESTO")) {
            return 1;
        } else if (strcmp(command, "INIZIO")) {
            return 2;
        } else if (strcmp(command, "PARCHEGGIO")) {
            return 3;
        } else {
            return -1;
        }
    }
}

int main(int argc, char const *argv[]) {
    int server_fd, client_fd, new_socket;
    int hmi, front, steer, rec; //components
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
    
    char dataCamera[BUFFER_SIZE] = {0};
    char line[2048];
    int buffer_length = 0;
    int bytes_received;
    int input = 0; 

    int speed = 0;

    hmi = 0;
    front = 1; 
    steer = 2;
    ssize_t bytesRead;
        
    for (int i = 0; i < MAX_CLIENTS; i++) {
        new_socket = accept(server_fd, (struct sockaddr *)&new_addr, &address_size);
        client_sockets[i] = new_socket;
        printf("Client %d connected\n", i + 1);
    }

    while(1) {

        input = getInput(client_sockets[hmi]); 
        if(input == -1) {
            wait(NULL);
        } else {
            printf("Comando: %d\n", input);
        }

        // while((recv(client_sockets[front], dataCamera, sizeof(dataCamera), 0)) > 0) {
        //     printf("Buffer: %s\n", dataCamera);
        //     if(strcmp(dataCamera, "SINISTRA") || strcmp(dataCamera, "DESTRA") ) {  

        //         write(client_sockets[steer], dataCamera, strlen(dataCamera));
        //         printf("Sent message to Steer\n");
        //     }
        // }

        // while( (bytesRead = read ( client_sockets[front],dataCamera, sizeof(dataCamera))) > 0);
        //     printf("Data: %s", dataCamera);

        //wait processo

    }
    close(server_fd);

    return 0;
}
