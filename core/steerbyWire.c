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

void print4sec(FILE *file, char command[]){
    int t = 1;
    while( t <= TIME ) {
        fputs(command, file);
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

    while ((bytesRead = read ( clientFd, command, sizeof(command))) > 0) {
           fputs(command, file);
        int value = strcmp(command, dx);
        if (value == 0) {
            print4sec(file, destra);
            memset(command,0,sizeof(command));
        } else {
            print4sec(file, sinistra);
            memset(command,0,sizeof(command));
        }
    } 
}

int main(int argc, char *argv[]) {
    int clientFd, serverLenght, connection;
    struct sockaddr_un server_addr;
    struct sockaddr* serverPTRAddr = ( struct sockaddr* ) &server_addr;
    serverLenght = sizeof( server_addr);
    //char command[100] = {0};

    FILE *file = fopen("steer.log", "a");
    if (file == NULL) {
        printf("Error to open file steer.log\n");
        exit(1); 
    }

    clientFd = socket( AF_UNIX, SOCK_STREAM, 0 );
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, SOCKET_NAME);

    while (connect( clientFd, serverPTRAddr, serverLenght) < 0) {
        printf("Connection problem, retrying ...\n");
        sleep(3);
    }
    
    printf("Connected to %s\n", SOCKET_NAME);
    
            // if (read( clientFd, command, sizeof(command))>0) {
            //     fputs(command, file);
            // }

    while(1) {
        steer_by_wire(clientFd, file);
    }

    fclose(file);
    close(clientFd);
    exit(0);
}







