#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#define DEF_PROTOCOL 0

int crea_socket(int serverFd, char* socketname){
    struct sockaddr_un serverAddress; //da non assegnare
    struct sockaddr_un clientAddress; //da non assegnare
    struct sockaddr* serverPtr; //puntatore indirizzo server
    struct sockaddr* clientPtr; //puntatore indirizzo client

    serverPtr = (struct sockaddr*) &serverAddress; //cast a sockaddr* della dereferenza dell'indirizzo server
    clientPtr = (struct sockaddr*) &clientAddress; //cast a sockaddr* della dereferenza dell'indirizzo client

    unsigned int serverLen = sizeof(serverAddress);  //dimensione indirizzo server
    unsigned int clientLen = sizeof(clientAddress);  //dimensione indirizzo client

    serverFd = socket(AF_UNIX, SOCK_STREAM, DEF_PROTOCOL);  //inizializzazione del socket
    serverAddress.sun_family = AF_UNIX;                     //assegno al socket la famiglia
    strcpy(serverAddress.sun_path, socketname);     
    unlink(socketname);                            
    bind(serverFd, serverPtr, serverLen);                 
    listen(serverFd, 5);                                    
    return accept(serverFd, clientPtr, &clientLen); 
}

void run_ecu(){
    int serverFd, clientFd;
    while(1){
        clientFd = crea_socket(serverFd, "socket_input");
        if(fork() == 0){
            printf("Connesso\n");
            char* comando;
            read(clientFd, comando, strlen(comando));
            printf("%s", comando);
            exit(0);
        }else{
            printf("Errore ... Nuovo tentativo tra 1 sec\n");
        }
        sleep(1);
    } 
}

int main(){
    while(1){
        if(fork() == 0){
            printf("Eseguo Figlio ECU\n");
            run_ecu();
            printf("Termino Figlio ECU\n");
            sleep(1);
        }else{
            printf("Padre ECU\n");
            sleep(1);
        }
    }
}

