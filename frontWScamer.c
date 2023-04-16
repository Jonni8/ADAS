#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *fdr = fopen("frontCamera.data", "r");
    char buffer[1024];
    char *data;
    if(fdr == NULL) {
        printf(" Can't open the file");
    }

    fseek(fdr, 0, SEEK_END);
    int lenght = ftell(fdr);
    fseek(fdr, 0, SEEK_SET);
    char *string = malloc(sizeof(char) * (lenght+1));
    
    FILE *fdw = fopen("camera.data", "w");
    char c;
    int i = 0;
    while( (c = fgetc(fdr)) != EOF ) {
        string[i] = c;
        putc(string[i], fdw);
        i++;
        //TODO: prendere ogni valore di ogni riga e poi fare una Write nel file camera.log
    }

    string[i] = '\0';

    fclose(fdr);
    fclose(fdw);
    return 0;
}

