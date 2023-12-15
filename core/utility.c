#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int readMessage(int file_descriptor, char *message) {
    do {
        if(read(file_descriptor, message, 1) < 0) {
            return -1;
        }
     } while(*message++ != '\0');
    return 0;
}

void printInFile(FILE *file, char *command) {
    fprintf(file, "%s", command);
    fflush(file);
}

int isListening(int socket) {
    int isListening;
    if (read(socket, &isListening, sizeof(isListening)) < 0) {
        perror("Errore durante l'ascolto\n");
        exit(EXIT_FAILURE);
    }
    return isListening;
}