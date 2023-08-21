#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#define DEF_PROTOCOL 0
#define MAX_CLIENT 10

int client_socket[MAX_CLIENT];
int num_client;

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

int createSocket(int serverFd, char* socketname){
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

int main(){
    char* comando = "NESSUNO";
    int serverFd, clientFd, throttleFd;

    while(1){
        clientFd = createSocket(serverFd, "socket_input");
        if(fork() == 0){
            comando = "INCREMENTO 5";
        }
            
        throttleFd = createSocket(serverFd, "socket_throttle");
        if(fork() == 0){
            write(throttleFd, comando, strlen(comando) + 1);
        }
        close(clientFd);
        close(throttleFd);
        sleep(1);
    }

    /*while(clientFd = createSocket(serverFd, "socket_input")){
        if(fork() == 0){
            close(serverFd);
        }else{
            while(throttleFd = createSocket(serverFd, "socket_throttle")){
                if(fork() == 0){
                    close(serverFd);
                }else{
                    close(clientFd);
                }
            }
        }
    }
    return 0;*/
}