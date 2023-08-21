#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h> 
#include <time.h>
#include "printFor4Sec.h"
#define TIME 4

char sinistra[] = "STO GIRANDO A SINISTRA\n";
char destra[] = "STO GIRANDO A DESTRA\n";

int checkCommand(char command[]) {
    if (command[0] == '\n' || command[0] == '\0') {
        return 1;
    } else {
        return 0;
    }
}

void print4sec(FILE *f, const char string[]){
    int t = 1;
    while( t <= TIME ) {
        fputs(string, f);
        t++;
        sleep(1);
    }
}

void steer_by_wire (char command[], FILE *file) {
    const char dx[] = "DESTRA";
    const char sx[] = "SINISTRA";
    const char no_action[] = "NO ACTION";
    if ( checkCommand(command) == 1) {
        while(checkCommand(command) == 1) {
            fputs("NO ACTION", file);
        }
    } 
    int value = strcmp(command, dx);
    if ( value == 0 ) {
        print4sec(file, destra);
        fclose(file);
    } else {
        print4sec(file, sinistra);
        fclose(file);
    } 
}

int main(int argc, char *argv[]) {
    char *command = argv[1];
    FILE *fdw = fopen("steer.log", "w");
    if (fdw == NULL) {
        printf("Error to open file steer.log");
        exit(1); 
    }
    int pid = fork();
    if(pid == 0) { // figlio
        steer_by_wire(command, fdw);
        exit(0);
    } else if (pid > 0) { //padre
        wait(NULL);
        //printf("Comando figlio eseguito\n");
        exit(0);
    } else {
        printf("Error in process pid= %d", pid);
        exit(1);
    }
}







