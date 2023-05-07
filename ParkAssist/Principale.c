#include<stdio.h>
#include <stdlib.h>

int main(){
   FILE *file;
    char filename[] = "testo8byte.txt"; // Sostituisci con il nome del tuo file
    unsigned char buffer[8]; // Buffer per memorizzare 8 byte letti dal file

    file = fopen(filename, "r"); // Apri il file in modalità lettura binaria (rb)
    if (file == NULL) {
        printf("Impossibile aprire il file %s\n", filename);
        exit(1);
    }

    fread(buffer, sizeof(int), 8, file); // Leggi 8 byte dal file, ignorando il valore restituito da fread()

    fclose(file); // Chiudi il file

    printf("8 byte letti dal file:\n");
    for (int i = 0; i < 8; i++) {
        printf("%d ", buffer[i]);
    }
    printf("\n");

    return 0;
}

