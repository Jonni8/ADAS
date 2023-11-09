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

void print4sec(FILE *f, const char string[]){
    int t = 1;
    while( t <= TIME ) {
        fputs(string, f);
        t++;
        sleep(1);
    }
}

void steer_by_wire (int clientFd, FILE *file) {
    const char dx[] = "DESTRA";
    const char sx[] = "SINISTRA";
    const char no_action[] = "NO ACTION";
    char command[100] = {0};

    while ((recv(clientFd, command, sizeof(command), 0)) > 0) {
        printf("Command: %s", command);
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
    char *command = argv[1];

    FILE *fdw = fopen("steer.log", "w");
    if (fdw == NULL) {
        printf("Error to open file steer.log");
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
    
    while(1) {
         steer_by_wire(clientFd, fdw);
    }

    fclose(fdw);
    close(clientFd);
    exit(0);
}






