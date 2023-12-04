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

#define MAX_CLIENTS 3
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
    int hmi, front, steer, rec, throttle, brake; //components
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
    char command[16];
    int buffer_length = 0;
    int bytes_received;
    int input = 0; 

    int speed = 0;

    //Da Cambiare!!
    hmi = 0; 
    throttle = 1;
    brake = 2;
    steer = 3;
    front = 9;


    ssize_t bytesRead;
    int i = 1;

    for (int i = 0; i < MAX_CLIENTS; i++) {
        new_socket = accept(server_fd, (struct sockaddr *)&new_addr, &address_size);
        client_sockets[i] = new_socket;
        printf("Client %d connected\n", i + 1);
    }

    while (1) {
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
            } else {
                printf("Other command\n");
            }
        }

        while((bytesRead = read(client_sockets[front], dataCamera, sizeof(dataCamera))) > 0) {
            if(strcmp(dataCamera, "SINISTRA\n") == 0 || strcmp(dataCamera, "DESTRA\n") == 0) {  
                write(client_sockets[steer], dataCamera, strlen(dataCamera)+1);
            }  else if(strlen(dataCamera) <= 3){
                int times = 0;
                int current_speed = atoi(dataCamera);
                if(current_speed > speed){
                    printf("Sent message to Throttle: %s\n", dataCamera);
                    char* c = "INCREMENTO 5";
                    write(client_sockets[throttle],c, strlen(c)+1);
                    speed += 5;
                }else if(current_speed < speed){
                    times = (speed - current_speed) / 5;
                    if(times-- > 0){
                        printf("Sent message to Brake: %s\n", dataCamera);
                        char* c = "FRENO 5";
                        write(client_sockets[brake],c, strlen(c)+1);
                        speed -= 5;
                    }
                }
            } else {
                write(client_sockets[steer], dataCamera, strlen(dataCamera)+1);
                i++;
            }
        }

        //wait processo

    }

    close(server_fd);

    return 0;
}
