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

int checkCommand(char command[]) {
    if (command[0] == '\n' || command[0] == '\0') {
        return 1;
    } else {
        return 0;
    }
}

void print4sec(FILE *f, const char string[]){
    int t = 1;
    while( t <= TIME ) {
        fputs(string, f);
        t++;
        sleep(1);
    }
}

void steer_by_wire (char command[], FILE *file) {
    const char dx[] = "DESTRA";
    const char sx[] = "SINISTRA";
    const char no_action[] = "NO ACTION";
    // if ( checkCommand(command) == 1) {
    //     while(checkCommand(command) == 1) {
    //         fputs("NO ACTION", file);
    //     }
    // } 
    int value = strcmp(command, dx);
    if ( value == 0 ) {
        print4sec(file, destra);
        fclose(file);
    } else {
        print4sec(file, sinistra);
        fclose(file);
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
         steer_by_wire(command, fdw);
    }
    close(clientFd);
    exit(0);
}







