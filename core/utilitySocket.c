#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

void socketAuth(int *client_fd, int *serverLen, struct sockaddr_un *serverAddress, char *serverName) {
    *serverLen = sizeof(*serverAddress);
    *client_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    struct sockaddr_un tmp = {.sun_family = AF_UNIX};
    strcpy(tmp.sun_path, serverName);
    *serverAddress = tmp;
}

void connectToServer(int client_fd, struct sockaddr* serverAddress, int serverLen) {
    while(connect(client_fd, serverAddress, serverLen) == -1) {
          printf("Error connection, try in 3 sec\n");
          sleep(3);
    }
}