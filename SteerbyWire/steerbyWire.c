#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "printFor4Sec.h"

char sinistra[] = "STO GIRANDO A SINISTRA\n";
char destra[] = "STO GIRANDO A DESTRA\n";

void steer_by_wire (char command[], FILE *file) {
    const char dx[] = "DESTRA";
    const char sx[] = "SINISTRA";
    int value = strcmp(command, dx);
    if ( value == 0 ) {
        printFor4sec(file, destra);
        fclose(file);
    } else {
        printFor4sec(file, sinistra);
        fclose(file);
    }
}

void p(int pid) {
    time_t start, end;
    double elapsed, prev_elapsed = 0.0;
    time(&start);  
    do {
        time(&end);
        elapsed = difftime(end, start);
        if (elapsed >= prev_elapsed+1.0)
        {   
            printf("%d", pid);
            prev_elapsed = elapsed;
        }
    } while(elapsed < 4.0);
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
    } else { //padre
        wait(NULL);
        printf("Comando figlio eseguito\n");
        exit(0); 
    }
}







