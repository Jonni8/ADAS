#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/wait.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>
#include <arpa/inet.h>

#include "utility.h"

#define TIME 4
#define SOCKET_NAME "ADAS"

char *sinistra = "STO GIRANDO A SINISTRA\n";
char *destra = "STO GIRANDO A DESTRA\n";
char *no_action = "NO ACTION\n"; 

void print4sec(FILE *file, char *command){
    int t = 1;
    while( t <= TIME ) {
        printInFile(file, command);
        t++;
        sleep(1);
    }
    printf("End Write to File\n");
}

void steer_by_wire (int clientFd, FILE *file) {
    const char dx[] = "DESTRA\n";
    const char sx[] = "SINISTRA\n";
    char command[100] = {0};
    ssize_t bytesRead;

    while(1) {
        if (bytesRead = read(clientFd, command, sizeof(command)) > 0) {
            if (strcmp(command, dx) == 0) {
                print4sec(file, destra);
            } else if (strcmp(command, sx) == 0) {
                print4sec(file, sinistra);
            } else {
                printInFile(file, no_action);
            }
        }
    }
}

int main(int argc, char *argv[]) {

    int clientFd, serverLenght, connection;
    struct sockaddr_un server_addr;
    struct sockaddr* serverPTRAddr = ( struct sockaddr* ) &server_addr;
    serverLenght = sizeof( server_addr);
    
    clientFd = socket( AF_UNIX, SOCK_STREAM, 0 );
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, SOCKET_NAME);
    //char command[100] = {0};

    while (connect( clientFd, serverPTRAddr, serverLenght) < 0) {
        printf("Connection problem, retrying ...\n");
        sleep(3);
    }

    FILE *steer_log = fopen("steer.log","a");
    if (steer_log == NULL) {
        printf("Error to open file steer.log\n");
        fclose(steer_log);
        exit(1);
    }
    
    printf("Connected to %s\n", SOCKET_NAME);

    steer_by_wire (clientFd, steer_log);

    fclose(steer_log);
    close(clientFd);
    
    return 0;
}







