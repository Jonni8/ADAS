#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    FILE *fdr = fopen("demo.txt", "r");
   
    if(fdr == NULL) {
        printf(" Can't open the file");
    }

    fseek(fdr, 0, SEEK_END);
    int lenght = ftell(fdr);
    fseek(fdr, 0, SEEK_SET);
    char *string = malloc(sizeof(char) * (lenght+1));

    //FILE *fdw = fopen("camera.data", "w");
    char c;
    int i = 0;
    char buffer[256];
    char *currentString;
    int currentIndex = 0;
    int index = 0; 
    do {
        c = fgetc(fdr);
        string[i] = c;
        currentIndex = i;

        //printf("before i= %d \n", i);
        
        if( c == '\n') {
            index = (i - index) -1;
            int indexT = 0;
            // printf("current index= %d \n", index);
            // printf(" i= %d \n", i);
            while (indexT <= index)  {
                buffer[indexT] = string[indexT];
                printf("%s \n", buffer);
                indexT++;
            }
            index = index + 1;
        }
        // printf("after i= %d \n", i);
        i++;
    } while ( c != EOF);

    
    fclose(fdr);
    //fclose(fdw);
    return 0;
}

