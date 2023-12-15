#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>

#include "frontWScamera.h"
#include "utility.h"
#define SOCKET_NAME "ADAS"
#define FRONT_CAMERA "dataFront.data"

void frontWindshieldCamera(int clientFd) {
    FILE *fileDataFront = fopen(FRONT_CAMERA, "r");
    FILE *fdw = fopen("log/camera.data", "w");
    char buffer[16];

    if(fileDataFront == NULL) {
        printf("Can't open the file\n");
        exit(1);
    }

    char *line;
    printf("Start reading from %s\n", FRONT_CAMERA);

    while(line = fgets(buffer, 16, fileDataFront)) {
        if(line == NULL) {
            //END of file
            break;
        }
        write(clientFd, line, strlen(line));
        printInFile(fdw, line);
        sleep(1);
    }

    fclose(fileDataFront);
    fclose(fdw);
}

int main() {

    int clientFd, serverLenght, connection;
    struct sockaddr_un server_addr;
    struct sockaddr* serverPTRAddr = ( struct sockaddr* ) &server_addr;
    serverLenght = sizeof( server_addr);

    clientFd = socket( AF_UNIX, SOCK_STREAM, 0 );
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, SOCKET_NAME);

    while (connect( clientFd, serverPTRAddr, serverLenght) < 0) {
        printf("Connection problem, retrying ...\n");
        sleep(3);
    }

    printf("Connected to %s\n", SOCKET_NAME);

    char active[8] = "0";
    ssize_t bytesRead;

    bytesRead = read(clientFd, active, strlen(active)+1);

    while (bytesRead < 0) {
        sleep(3);
    } 

    if(strcmp(active, "1") == 0) {
        frontWindshieldCamera(clientFd);
    }
    printf("End read from %s\n", FRONT_CAMERA);

    close(clientFd);
    exit(0);
}
