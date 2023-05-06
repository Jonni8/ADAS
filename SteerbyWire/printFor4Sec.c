#include <stdio.h>
#include <time.h>

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