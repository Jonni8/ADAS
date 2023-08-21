#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#define DEF_PROTOCOL 0

void connetti_input_server(){
    int result; //File descriptor per socket ed il risultato della connessione
    struct sockaddr_un serverAddress; //da non assegnare
    struct sockaddr* serverPtr; //puntatore indirizzo server

    serverPtr = (struct sockaddr*) &serverAddress; //cast a sockaddr* della dereferenza dell'indirizzo server
    unsigned int serverLen = sizeof(serverAddress);  //dimensione server

    int clientFd = socket(AF_UNIX, SOCK_STREAM, DEF_PROTOCOL);  //inizializzazione del socket
    serverAddress.sun_family = AF_UNIX;                     //assegno al socket la famiglia
    strcpy(serverAddress.sun_path, "../Controllo/socket_input");      //assegno al socket il nome "throttleControl"

    do{
        printf("Connessione in corso ...\n");
        char* comando;
        int i = 0;
        result = connect(clientFd, serverPtr, serverLen);
        if(result == -1){
            printf("Errore ... Nuovo tentativo tra 1 sec\n");
        }else{
            printf("Connesso\n");
            comando = "PROVA";
            write(clientFd,comando,strlen(comando));
            exit(0);
        }
        sleep(1);
    } while(result == -1);
}

int main(int argc, char *argv[]){
    if(strcmp(argv[1], "NORMALE") == 0){
        printf("NORMALE\n");
        while(1){
            if(fork() == 0){
                connetti_input_server();
                exit(0);
            }else{
                sleep(1);
            }
        }
    }
    if(strcmp(argv[1], "ARTIFICIALE") == 0){
        printf("ARTIFICIALE");
    }
    return 0;
}