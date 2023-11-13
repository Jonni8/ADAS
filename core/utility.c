#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int readMessage(int file_descriptor, char *message) {
    while(read(file_descriptor, message, sizeof(message)) < 0) {
        return -1;
    }
    return 0;
}