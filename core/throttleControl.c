#include "throttleControl.h"

static void aggiustaOra(char* el);

char* oraCorrente(){
    struct tm* ptrOraCorrente;
    time_t t;
    t = time(NULL);
    ptrOraCorrente = localtime(&t);
    char* ora = asctime(ptrOraCorrente);
    aggiustaOra(ora);
    return ora;
}

static void aggiustaOra(char* el){
    int intervallo = (int)strcspn(el, "\n");
    for(int i = intervallo; i<(int)strlen(el); i++){
        el[i] = el[i+1];
    }
}

int leggi(int fd, char * str){
    long int n;
    do{
        n = read(fd,str,1);
    }while(n > 0 && *str++ != '\0');
    return (n > 0);
}

int main(){
    FILE* logFd = fopen("log/throttle.log","arw+");
    int result; //File descriptor per socket ed il risultato della connessione
    struct sockaddr_un serverAddress; //da non assegnare
    struct sockaddr* serverPtr; //puntatore indirizzo server

    serverPtr = (struct sockaddr*) &serverAddress; //cast a sockaddr* della dereferenza dell'indirizzo server
    unsigned int serverLen = sizeof(serverAddress);  //dimensione server

    int clientFd = socket(AF_UNIX, SOCK_STREAM, DEF_PROTOCOL);  //inizializzazione del socket
    serverAddress.sun_family = AF_UNIX;                     //assegno al socket la famiglia
    strcpy(serverAddress.sun_path, SOCKET_NAME);      //assegno al socket il nome

    do{
        printf("Connessione in corso ...\n");
        result = connect(clientFd, serverPtr, serverLen);
        if(result == -1){
            printf("Errore ... Nuovo tentativo tra 1 sec\n");
            sleep(1);
        }
    }while(result == -1);

    char str[200];
    
    while(1) {
        if (read(clientFd, str, sizeof(str)) > 0) {
            printf("%s ",str);
            printf("Evento registrato nel log\n"); //per debugging
            char* time = oraCorrente();
            char* message = " AUMENTO 5\n";
            char *increment = strcat(time,message);
            fseek(logFd,0,SEEK_END);
            //fwrite(t,1,strlen(t), logFd);
            printInFile(logFd,increment);
        }
    }
    
    fclose(logFd);
    close(clientFd);
    exit(0);
}