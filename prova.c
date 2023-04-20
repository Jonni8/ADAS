#include <stdio.h> 


int main() {
    char *string = "ciao";
    // char *p;
    // char *array[2];
    // array[0] = string;
    // p = *&string;
    // printf("string = %s \n", string);
    // printf("p = %s \n", p);
    // printf("array = %s \n", array[0]);
    printf("string = %s \n", string);
    string++;
    printf("string = %s \n", string);
}