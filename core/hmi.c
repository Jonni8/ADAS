#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define DEF_PROTOCOL 0
#define SOCKET_NAME "ADAS"

int main(){
    int result; //File descriptor per socket ed il risultato della connessione
    struct sockaddr_un serverAddress; //da non assegnare
    struct sockaddr* serverPtr; //puntatore indirizzo server

    serverPtr = (struct sockaddr*) &serverAddress; //cast a sockaddr* della dereferenza dell'indirizzo server
    unsigned int serverLen = sizeof(serverAddress);  //dimensione server

    int clientFd = socket(AF_UNIX, SOCK_STREAM, DEF_PROTOCOL);  //inizializzazione del socket
    serverAddress.sun_family = AF_UNIX;                     //assegno al socket la famiglia
    strcpy(serverAddress.sun_path, SOCKET_NAME);      //assegno al socket il nome "throttleControl"

    while (connect( clientFd, serverPtr, serverLen) < 0) {
        printf("Connection problem, retrying ...\n");
        sleep(3);
    }

    printf("Connected to %s\n", SOCKET_NAME);

    char comando[64];

    while(1) {
        scanf("%s", comando);
        if (strcmp(comando, "INIZIO") == 0) {
        }
        else if (strcmp(comando, "PARCHEGGIO") == 0) {
        }
        else if (strcmp(comando, "ARRESTO") == 0) {
        } else {
            printf("Command not valid");
        }
        
        write(clientFd, comando, strlen(comando) + 1);
    }
    return 0;
}

