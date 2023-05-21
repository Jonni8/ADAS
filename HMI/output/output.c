#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#define DEF_PROTOCOL 0

int leggi(int fd, char * str){
    long int n;
    do{
        n = read(fd,str,1);
    }while(n > 0 && *str++ != '\0');
    return (n > 0);
}

void leggiLog(int fd){
    char str[200];
    while(leggi(fd,str)) //mettere il punto e virgola per leggere solo l'ultima riga
    printf("%s\n",str);
}

int main(){
    int serverFd, clientFd; //File descriptor per socket con rispettiva lunghezza
    char str[200];
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
    strcpy(serverAddress.sun_path, "../ECU");      //assegno al socket il nome "throttleControl"
    unlink("../ECU");                              //elimino il file se esiste
    bind(serverFd, serverPtr, serverLen);                   //creo il file e lo associo al server
    listen(serverFd, 5);                                    //numero di connessioni possibile
    while(1){                                               //loop eterno    
        clientFd = accept(serverFd, clientPtr, &clientLen); //attendi connessione da parte del client
        printf("In attesa di connessione ... \n");
        if(fork() == 0){                                    //creo processo per inviare al client
            printf("Leggo dati ... \n");
            leggiLog(clientFd);
            close(clientFd);
        }else{
            close(clientFd);
        }
    }
    return 0;
}