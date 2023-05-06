#include "throttleControl.h"

static void aggiustaOra(char* el);

char* oraCorrente(){
    struct tm* ptrOraCorrente;
    time_t t;
    t = time(NULL);
    ptrOraCorrente = localtime(&t);
    char* ora = asctime(ptrOraCorrente);
    aggiustaOra(ora);
    return ora;
}

static void aggiustaOra(char* el){
    int intervallo = (int)strcspn(el, "\n");
    for(int i = intervallo; i<(int)strlen(el); i++){
        el[i] = el[i+1];
    }
}

void redigiLog(int fd){
    char* t = oraCorrente();
    char z[] = " INCREMENTO 5";
    t = strcat(t,z);
    long int n = write(fd,t,strlen(t) + 1);
    printf("%s\n",t);
    if(n == -1){
        exit(-1);
    }
}