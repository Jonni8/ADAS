#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/un.h>
#include <ctype.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/unistd.h>

#include "utility.h"

#define MAX_CLIENTS 4
#define SOCKET_NAME "ADAS"
#define BUFFER_SIZE 2048
#define ACTIVE_FRONT "1"
#define DEACTIVE_FRONT "0"

int getInput(int fd, char *command) {
    if(readMessage(fd, command) == 0) {
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

int isNumber(char *number) {
    for(int i = 0; number[i] != '\0'; i++) {
        if(!isdigit(number[i])) {
            return 0;
        }
        return 1;
    }
}

int main(int argc, char const *argv[]) {
    int server_fd, client_fd, new_socket;
    int hmi, front, steer, rec, throttle, brake, parkassist; //components
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
    /* command receive from hmi */
    int input = 0; 

    int speed = 0;

    //Da Cambiare!!
    hmi = 0; 
    throttle = 1;
    //brake = 2;
    steer = 2;
    //parkassist = 4;
    front = 3;

    int i = 1;

    for (int i = 0; i < MAX_CLIENTS; i++) {
        new_socket = accept(server_fd, (struct sockaddr *)&new_addr, &address_size);
        client_sockets[i] = new_socket;
        printf("Client %d connected\n", i + 1);
    }

    while (1) {
        /***ricevo dati dall'hmi***/
            char dataInput[8];
            input = getInput(client_sockets[hmi], command);
            printf("Comando: %d--\n", input);
            if (input == -1) {
                wait(NULL);
            } else {
                if (input == 2) {
                    
                    sprintf(dataInput, "%d", input);
                    write(client_sockets[front], ACTIVE_FRONT, 1);
                    printf("Sent message to front: %s\n", command);

                    while ((read(client_sockets[front], dataCamera, sizeof(dataCamera))) > 0) {
                        if (strcmp(dataCamera, "SINISTRA\n") == 0 || strcmp(dataCamera, "DESTRA\n") == 0) {
                            write(client_sockets[steer], dataCamera, strlen(dataCamera) + 1);
                            printf("Sent Message to steer\n");
                            memset(dataCamera, 0, sizeof(dataCamera));
                        } else if (isNumber(dataCamera) == 1) {
                            int times = 0;
                            int current_speed = atoi(dataCamera);

                            write(client_sockets[steer], dataCamera, strlen(dataCamera) + 1);
                            
                            if (current_speed > speed) {
                                printf("Sent message to Throttle: %s\n", dataCamera);
                                char *c = "INCREMENTO 5";
                                write(client_sockets[throttle], c, strlen(c) + 1);
                                speed += 5;
                            } else if (current_speed < speed) {
                                times = (speed - current_speed) / 5;
                                if (times-- > 0) {
                                    printf("Sent message to Brake: %s\n", dataCamera);
                                    char *c = "FRENO 5";
                                    write(client_sockets[brake], c, strlen(c) + 1);
                                    speed -= 5;
                                }
                            } 
                            printf("SPEED: %d\n", speed);
                        } else if (strcmp(dataCamera, "PARCHEGGIO\n") == 0 ) {
                            printf("PARKING\n");
                        } else {
                            write(client_sockets[steer], dataCamera, strlen(dataCamera) + 1);
                            printf("Sent Message to steer\n");
                            i++;
                        }
                    }
                } else if (input == 3) {
                    printf("---PARK ASSIST---\n");
                } else {
                    printf("ARRESTO\n");
                }
            }

        close(server_fd);
        return 0;
    }
}

