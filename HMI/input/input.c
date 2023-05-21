#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#define DEF_PROTOCOL 0

int main(){
    int clientFd;
    char comando[100];
    do{
        scanf("%s",comando);
        if(strcmp(comando,"INIZIO") == 0){
            printf("Inizio \n");
        }else if(strcmp(comando,"PARCHEGGIO") == 0){
            printf("Parcheggio \n");
        }else if(strcmp(comando,"ARRESTO") == 0){
            printf("Arresto \n");
        }
        connectAndSend(clientFd, comando);
    }while(strcmp(comando,"EXIT") != 0);
}

int connectAndSend(int clientFd, char* comando){
    int result; //File descriptor per socket ed il risultato della connessione
    struct sockaddr_un serverAddress; //da non assegnare
    struct sockaddr* serverPtr; //puntatore indirizzo server

    serverPtr = (struct sockaddr*) &serverAddress; //cast a sockaddr* della dereferenza dell'indirizzo server
    unsigned int serverLen = sizeof(serverAddress);  //dimensione server

    clientFd = socket(AF_UNIX, SOCK_STREAM, DEF_PROTOCOL);  //inizializzazione del socket
    serverAddress.sun_family = AF_UNIX;                     //assegno al socket la famiglia
    strcpy(serverAddress.sun_path, "../ECU");      //assegno al socket il nome "throttleControl"

    do{
        printf("Connessione in corso ...\n");
        result = connect(clientFd, serverPtr, serverLen);
        if(result == -1){
            printf("Errore ... Nuovo tentativo tra 5 sec\n");
            sleep(5);
        }else{
            write(clientFd,comando,strlen(comando)+1);
        }
    } while(result == -1);
    return result;
}

/*
CLIENT
int initClient(){
    int clientFd, result; //File descriptor per socket ed il risultato della connessione
    struct sockaddr_un serverAddress; //da non assegnare
    struct sockaddr* serverPtr; //puntatore indirizzo server

    serverPtr = (struct sockaddr*) &serverAddress; //cast a sockaddr* della dereferenza dell'indirizzo server
    unsigned int serverLen = sizeof(serverAddress);  //dimensione server

    clientFd = socket(AF_UNIX, SOCK_STREAM, DEF_PROTOCOL);  //inizializzazione del socket
    serverAddress.sun_family = AF_UNIX;                     //assegno al socket la famiglia
    strcpy(serverAddress.sun_path, "throttleControl");      //assegno al socket il nome "throttleControl"

    do{
        printf("Connessione in corso ...\n");
        result = connect(clientFd, serverPtr, serverLen);
        if(result == -1){
            printf("Errore ... Nuovo tentativo tra 5 sec\n");
            sleep(1);
        }
    } while(result == -1);
    printf("Lettura in corso ... \n");
    leggiLog(clientFd);
    printf("Lettura avvenuta ... \n");
    close(clientFd);
}
SERVER
int main(){
    int serverFd, clientFd; //File descriptor per socket con rispettiva lunghezza
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
    strcpy(serverAddress.sun_path, "client/throttleControl");      //assegno al socket il nome "throttleControl"
    unlink("client/throttleControl");                              //elimino il file se esiste
    bind(serverFd, serverPtr, serverLen);                   //creo il file e lo associo al server
    listen(serverFd, 5);                                    //numero di connessioni possibile
    while(1){                                               //loop eterno    
        clientFd = accept(serverFd, clientPtr, &clientLen); //attendi connessione da parte del client
        printf("In attesa di connessione ... \n");
        if(fork() == 0){                                    //creo processo per inviare al client
            printf("Invio dati ... \n");
            eseguiOperazione(clientFd);
            close(clientFd);
            exit(0);
        }else{
            close(clientFd);
        }
        sleep(1);
    }
    return 0;
}*/

