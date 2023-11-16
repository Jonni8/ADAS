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

#define TIME 4
#define SOCKET_NAME "ADAS"

char sinistra[] = "STO GIRANDO A SINISTRA\n";
char destra[] = "STO GIRANDO A DESTRA\n";

void print4sec(FILE *file, char *command){
    int t = 1;
    while( t <= TIME ) {
        fprintf(file,"%s", sinistra);
        fflush(file);
        printf("Stampato\n");
        t++;
        sleep(1);
    }
    printf("End Write to File\n");
}

void steer_by_wire (int clientFd, FILE *file) {
    const char dx[] = "DESTRA";
    const char sx[] = "SINISTRA";
    const char no_action[] = "NO ACTION";
    char command[100] = {0};
    ssize_t bytesRead;

    printf("qui");
    if (read( clientFd, command, sizeof(command)) > 0) {
        int value = strcmp(command, dx);
        if (strcmp(command, dx) == 0) {
            print4sec(file, destra);
        } else if (strcmp(command, sx) == 0) {
            print4sec(file, sinistra);
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

    FILE *file = fopen("steer.log","a");
    if (file == NULL) {
        printf("Error to open file steer.log\n");
        fclose(file);
        exit(1);
    }
    
    printf("Connected to %s\n", SOCKET_NAME);

    const char dx[] = "DESTRA";
    const char sx[] = "SINISTRA";
    const char no_action[] = "NO ACTION";
    char command[100] = {0};
    ssize_t bytesRead;

    // read(clientFd, command, sizeof(command));

    while(1) {

        if (bytesRead=read(clientFd, command, sizeof(command)) > 0) {
            printf("Read: %s\n", command);
            if (strcmp(command, dx) == 0) {
                printf("dx\n");
                print4sec(file, destra);
            } else if (strcmp(command, sx) == 0) {
                printf("sx\n");
                print4sec(file, sinistra);
            }
        } 
    }

    fclose(file);
    close(clientFd);
    
    return 0;
}







