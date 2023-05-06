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