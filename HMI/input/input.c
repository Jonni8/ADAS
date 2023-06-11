#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#define DEF_PROTOCOL 0
/*
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
    strcpy(serverAddress.sun_path, "../../ECU/socket_ecu");      //assegno al socket il nome "throttleControl"

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
*/
int main(){
    int result; //File descriptor per socket ed il risultato della connessione
    struct sockaddr_un serverAddress; //da non assegnare
    struct sockaddr* serverPtr; //puntatore indirizzo server

    serverPtr = (struct sockaddr*) &serverAddress; //cast a sockaddr* della dereferenza dell'indirizzo server
    unsigned int serverLen = sizeof(serverAddress);  //dimensione server

    int clientFd = socket(AF_UNIX, SOCK_STREAM, DEF_PROTOCOL);  //inizializzazione del socket
    serverAddress.sun_family = AF_UNIX;                     //assegno al socket la famiglia
    strcpy(serverAddress.sun_path, "../../ECU/socket_input");      //assegno al socket il nome "throttleControl"

    do{
        printf("Connessione in corso ...\n");
        result = connect(clientFd, serverPtr, serverLen);
        if(result == -1){
            printf("Errore ... Nuovo tentativo tra 5 sec\n");
            sleep(5);
        }else{
            char comando[100];
            do{
                scanf("%s",comando);
                if(strcmp(comando,"INIZIO\n") == 0){
                    printf("Inizio \n");
                }else if(strcmp(comando,"PARCHEGGIO\n") == 0){
                    printf("Parcheggio \n");
                }else if(strcmp(comando,"ARRESTO\n") == 0){
                    printf("Arresto \n");
                }
                write(clientFd,comando,strlen(comando)+1);
            }while(strcmp(comando,"EXIT") != 0);
        }
    } while(result == -1);
    return result;
}

