#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#define DEVURANDOM "/dev/urandom"
#define FILELOG "testo8byte.txt"

void getUrandomName(const char *arg, char *urandomName) {
    if (strcmp(arg, "NORMALE") == 0) {
        snprintf(urandomName, 128, DEVURANDOM);
    } else {
        snprintf(urandomName, 128, "./urandomARTIFICIALE.binary");
    }
}

//172A,D693,0000,BDD8,FAEE,4300 --> 1D0BF4
bool controlloLongParkAssist(char * risultato_esadecimale){
    char *sub172A = strstr(risultato_esadecimale,"0x172A");
    char *subD693 = strstr(risultato_esadecimale,"0xD693");
    char *sub0000 = strstr(risultato_esadecimale,"0x0000");
    char *subBDD8 = strstr(risultato_esadecimale,"0xBDD8");
    char *subFAEE = strstr(risultato_esadecimale,"0xFAEE");
    char *sub4300 = strstr(risultato_esadecimale,"0x4300");

    if(sub172A != NULL || subD693 != NULL || sub0000 != NULL || subBDD8 != NULL || subFAEE != NULL || sub4300!= NULL){
        return true;
    }else{
        return false;
    }
}

void scriviSulFile(FILE *fileLog, char *testo){
    if(fileLog != NULL){
        fprintf(fileLog,"%s\n",testo);
        fflush(fileLog);
    }else{
        printf("Errore scrittura nel file: puntatore uguale a null");
    }
}

void parkAssist(FILE *urandom, FILE *fileLog , char *risultato_esadecimale){
    unsigned char buffer[8]; // Buffer per memorizzare 8 byte letti dal file

    fread(buffer, 1, 8, urandom); // Leggi 8 byte dal file, ignorando il valore restituito da fread()

    unsigned long *liValore = (unsigned long*)buffer;
    unsigned long lEsadecimale = *liValore;
    sprintf(risultato_esadecimale,"0x%lX",lEsadecimale);
    scriviSulFile(fileLog, risultato_esadecimale);
}

int main(){
    
    FILE *urandom = fopen(DEVURANDOM, "r");
    FILE *fileLog = fopen(FILELOG,"a");
    char risultato_esadecimale[20];
    int count = 0; 

    if (urandom == NULL) {
        printf("Impossibile aprire il file %s", DEVURANDOM);
        exit(1);
    }

    if (fileLog == NULL) {
        printf("Errore all'apertura di %s", FILELOG);
        exit(1);
    }
    while(count < 30) {
        parkAssist(urandom, fileLog, risultato_esadecimale);
        bool esito = controlloLongParkAssist(risultato_esadecimale);

        printf("%s - %d\n",risultato_esadecimale, esito);
        count++;
        sleep(1);
    }

    fclose(urandom);
    fclose(fileLog);
    return 0;
}







