#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "ParkAssist.h"

void scriviSulFileConData(char testo[], char nomefileTxt[]);
bool controlloLongParkAssist(char * risultato_esadecimale);


int main(){

    char risultato_esadecimale[20];
    parkAssist(risultato_esadecimale);
    printf("%s-",risultato_esadecimale);
    bool esito = controlloLongParkAssist(risultato_esadecimale);
    printf("%d", esito);


    return 0;
}

void parkAssist(char * risultato_esadecimale){

    FILE *file;
    char filename[] = "/dev/urandom"; // Sostituisci con il nome del tuo file
    unsigned char buffer[8]; // Buffer per memorizzare 8 byte letti dal file

    file = fopen(filename, "r"); // Apri il file in modalità lettura binaria (rb)
    if (file == NULL) {
        perror("Impossibile aprire il file /dev/urandom");
        exit(1);
    }

    fread(buffer, 1, 8, file); // Leggi 8 byte dal file, ignorando il valore restituito da fread()

    fclose(file); // Chiudi il file

    unsigned long *liValore = (unsigned long*)buffer;
    unsigned long lEsadecimale = *liValore;
    sprintf(risultato_esadecimale,"0x%lX",lEsadecimale);

    scriviSulFile(risultato_esadecimale,"testo8byte.txt");

    return risultato_esadecimale;

}


void scriviSulFile(char testo[], char nomefileTxt[]){
    FILE *pFile;

    //modalità scrittura
    pFile = fopen(nomefileTxt,"a"); //modalità append aggiunge perché se utilizzo "w" cancella e riscrive

    //il prof non parla dell'utilizzo di fprintf per la scrittura di un file
    //ma solo di fwrite. (possibilià di riscrivere il codice)
    if(pFile != NULL){
        fprintf(pFile,"%s\n",testo);
        fclose(pFile);
    }else{
        printf("Errore scrittura nel file: puntatore uguale a null");
    }
}

//172A,D693,0000,BDD8,FAEE,4300 --> 1D0BF4
bool controlloLongParkAssist(char * risultato_esadecimale){
    char *sub172A = strstr(risultato_esadecimale,"172A");
    char *subD693 = strstr(risultato_esadecimale,"D693");
    char *sub0000 = strstr(risultato_esadecimale,"0000");
    char *subBDD8 = strstr(risultato_esadecimale,"BDD8");
    char *subFAEE = strstr(risultato_esadecimale,"FAEE");
    char *sub4300 = strstr(risultato_esadecimale,"4300");

    if(sub172A != NULL || subD693 != NULL || sub0000 != NULL || subBDD8 != NULL || subFAEE != NULL || sub4300!= NULL){
        return true;
    }else{
        return false;
    }
}




