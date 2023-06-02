#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/my_socket"

int main(int argc,char*argv[]) {
    int clientSocket;
    struct sockaddr_un serverAddr;
    char buffer[] = "INIZIO!";

    // Creazione del socket
    clientSocket = socket(PF_UNIX, SOCK_STREAM, 0);

    // Configurazione del socket
    serverAddr.sun_family = AF_UNIX;
    strcpy(serverAddr.sun_path, SOCKET_PATH);

    // Connessione al server
    connect(clientSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

    // Invio del messaggio
    send(clientSocket, buffer, strlen(buffer), 0);

    // Chiudere il socket
    close(clientSocket);

    return 0;
}
