#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int readMessage(int file_descriptor, char *message) {
    while(read(file_descriptor, message, sizeof(message)+1) < 0) {
        return -1;
    }
    return 0;
}

void printInFile(FILE *file, char *command) {
    fprintf(file, "%s", command);
    fflush(file);
}