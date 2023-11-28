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

#define MAX_CLIENTS 2
#define SOCKET_NAME "ADAS"
#define BUFFER_SIZE 2048
#define ACTIVE_FRONT "1"

int getInput(int fd, char *command) {
    if(readMessage(fd, command) == 0) {
        printf("C: %s\n", command);
       if (strcmp(command, "ARRESTO") == 0) {
            return 1;
        } else if (strcmp(command, "INIZIO") == 0) {
            return 2;
        } else if (strcmp(command, "PARCHEGGIO") == 0) {
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
    char command[256];
    int buffer_length = 0;
    int bytes_received;
    int input = 0; 

    int speed = 0;

    //Da Cambiare!!
    hmi = 0;
    front = 1; 
    steer = 2;

    ssize_t bytesRead;
    int i = 1;

    while(1) {
        for (int i = 0; i < MAX_CLIENTS; i++) {
            new_socket = accept(server_fd, (struct sockaddr *)&new_addr, &address_size);
            client_sockets[i] = new_socket;
            printf("Client %d connected\n", i + 1);
        }

        /***ricevo dati dall'hmi***/
        input = getInput(client_sockets[hmi], command);
        char dataInput[8]; 
        if(input == -1) {
            wait(NULL);
        } else {
            printf("Comando: %d--\n", input); 
            if (input == 2) {
                sprintf(dataInput, "%d", input);
                write(client_sockets[front], ACTIVE_FRONT, 1);
                printf("Sent message to front: %s\n", command);
            } else {
                printf("Other command\n");
            }

        }
                   
        // while(readMessage(client_sockets[front], command) == 0) {
        //     printf("Read: %s\n", command);
        //     printf("After reset: %s\n", dataCamera);
        // }

        // while((bytesRead = read(client_sockets[front], dataCamera, sizeof(dataCamera))) > 0) {
        //     printf("BYTES: %ld\n", bytesRead);

        //     if(strcmp(dataCamera, "SINISTRA\n") == 0) {  
        //         write(client_sockets[steer], dataCamera, strlen(dataCamera)+1);
        //         printf("Sent message to Steer: %s\n", dataCamera);
        //     } else if( strcmp(dataCamera, "DESTRA\n") == 0) {
        //         write(client_sockets[steer], dataCamera, strlen(dataCamera)+1);
        //         printf("Sent message to Steer: %s\n", dataCamera);
        //         memset(dataCamera, 0, sizeof(dataCamera));
        //     } else {
        //         printf("Another command - %d\n", i);
        //         i++;
        //     }

        // }

        //wait processo

    }

    close(server_fd);

    return 0;
}
