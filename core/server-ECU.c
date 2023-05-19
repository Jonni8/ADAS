#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>

int readstring(int fd, char *bufferData) {
    int n; 
    do {
        n = read (fd, bufferData, 1);
    } while ( n > 0 && *bufferData++ != '\0' );
    return( n > 0); 
}

int readData(int fd) {
    char bufferData[200];
    while(readstring( fd, bufferData)) {
        printf("%s \n", bufferData);
    }
}

int main(int argc, char const *argv[]) {

    int server_fd, client_fd, serverLenght, clientLenght;
    int steer_pid;
    struct sockaddr_un server_adr, client_addr;
    struct sockaddr *serverPtr, *clientPtr; 
    char client_message[2000];

    serverPtr = (struct sockaddr*) &server_adr;
    serverLenght = sizeof(server_adr);

    clientPtr = (struct sockaddr*) &client_addr;
    clientLenght = sizeof(clientLenght);
    
    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if( server_fd == -1) {
        printf("Could not create socket server");
        exit(1);
    }
    server_adr.sun_family = AF_UNIX;
    strcpy(server_adr.sun_path, "ECU"); //set name del socket
    unlink("ECU");
    bind(server_fd, serverPtr, serverLenght);

    listen(server_fd, 20);

    while(1) {
        client_fd = accept(server_fd, clientPtr, &clientLenght);
        printf("Connected ... \n");
        steer_pid = fork();
        if( steer_pid == 0 ) {
            // sendData(client_fd);
            
            readData(client_fd);
            close(client_fd);
            exit(0);
        } else {
        close(client_fd);
        }
    }
    return 0;
}
