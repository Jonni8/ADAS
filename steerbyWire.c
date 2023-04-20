#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

void printFor4sec(FILE *f, char *string) {
    time_t start, end;
    double elapsed, prev_elapsed = 0.0;
    time(&start);  
    do {
        time(&end);
        elapsed = difftime(end, start);
        if (elapsed >= prev_elapsed+1.0)
        {
            putc(*string, f);
            //printf("DESTRA \n");
            *string++;
            prev_elapsed = elapsed;
        }
    } while(elapsed < 4.0);
}

int main(int argc, char *argv[]) {
    int i = 1;
    char *command = argv[1];
    FILE *fdw = fopen("steer.log", "w");
    printf("%s \n",command);
    //const char *dx = "DESTRA"; 
    //int value = (char) strcmp(&command, dx);
    //printf("%d", &dx);
    
    printFor4sec(fdw, command);
    
    //steer_by_wheel(command);

   }

/*
void steer_by_wheel (char *command) {
    const char *dx = "DESTRA";
    const char *sx = "SINISTRA"; 
    int value = strcmp(command, dx);
    FILE *fdw = fopen("steer.log", "w");
    if ( value == 0 ) {
        printFor4sec(*dx, &fdw);
    } else {
        printFor4sec(*sx, &fdw);
    }

}
*/




