#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "printFor4Sec.h"

char sinistra[] = "STO GIRANDO A SINISTRA\n";
char destra[] = "STO GIRANDO A DESTRA\n";

void steer_by_wire (char command[]) {
    const char dx[] = "DESTRA";
    const char sx[] = "SINISTRA"; 
    FILE *fdw = fopen("steer.log", "w");
    int value = strcmp(command, dx);
    if ( value == 0 ) {
        printFor4sec(fdw, destra);
        fclose(fdw);
    } else {
        printFor4sec(fdw, sinistra);
        fclose(fdw);
    }
}

int main(int argc, char *argv[]) {
    char *command = argv[1];
    steer_by_wire(command);
   }







