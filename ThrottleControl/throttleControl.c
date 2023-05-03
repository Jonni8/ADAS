#include "throttleControl.h"

static void aggiustaOra(char* el);

char* oraCorrente(){
    struct tm* ptrOraCorrente;
    time_t t;
    t = time(NULL);
    ptrOraCorrente = localtime(&t);
    char* ora = asctime(ptrOraCorrente);
    aggiustaOra(ora);
    printf("Ora: %s",ora);
    return ora;
}

static void aggiustaOra(char* el){
    int intervallo = (int)strcspn(el, "\n");
    printf("Intervallo : %d",intervallo);
    printf("STR LEN PRE: %d",(int)strlen(el));
    for(int i = intervallo; i<(int)strlen(el); i++){
        el[i] = el[i+1];
    }
    printf("STR LEN POST: %d",(int)strlen(el));
}

void redigiLog(int fd, char* velocità){
    char* t = oraCorrente();
    char z[] = " INCREMENTO ";
    //char y[strlen(t)];
    t = strcat(t,z);
    //strcat(y,z);
    t = strcat(t,velocità);
    //write(fd,y,strlen(t) + strlen(z) + strlen(velocità));
    write(fd,t,strlen(t));
}