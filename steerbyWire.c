#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

char sinistra[] = "STO GIRANDO A SINISTRA\n";
char destra[] = "STO GIRANDO A DESTRA\n";

void printFor4sec(FILE *f, const char string[]) {
    time_t start, end;
    double elapsed, prev_elapsed = 0.0;
    time(&start);  
    do {
        time(&end);
        elapsed = difftime(end, start);
        if (elapsed >= prev_elapsed+1.0)
        {   
            fputs(string, f);
            prev_elapsed = elapsed;
        }
    } while(elapsed < 4.0);
}

void steer_by_wheel (char command[]) {
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
    steer_by_wheel(command);
   }







