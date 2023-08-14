#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include "frontWScamera.h"


void frontWindshieldCamera(int clientFd) {
    FILE *fdr = fopen("frontCamera.data", "r");
    char buffer[100];
    char sendToServer[100] = {0};
    int *p_sendToServer;
    if(fdr == NULL) {
        printf("Can't open the file\n");
        exit(1);
    }

    FILE *fdw = fopen("camera.data", "w");
    const char *c;
    while( (c = fgets(buffer, sizeof(buffer),fdr)) != NULL ) {
        p_sendToServer = c;
        write(clientFd, c , strlen(c)+1);
        fputs(c, fdw);   
    }
    fclose(fdr);
    fclose(fdw);
}

int main() {

    int clientFd, serverLenght, connection;
    struct sockaddr_un server_addr;
    struct sockaddr* serverPTRAddr = ( struct sockaddr* ) &server_addr;
    serverLenght = sizeof( server_addr);
    
    clientFd = socket( AF_UNIX, SOCK_STREAM, 0 );
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, "ECU");

    do { 
        connection = connect( clientFd, serverPTRAddr, serverLenght);
        if( connection == -1 ) {
            printf("Connection problem, retrying ...\n");
            sleep(1);
        }
    } while( connection == -1);
    frontWindshieldCamera(clientFd);
    close(clientFd);
    exit(0);
}
