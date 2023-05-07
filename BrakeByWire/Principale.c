#include <stdio.h>
#include <time.h>
#include "metodiUtilizzati.h"
//#include "velocitaGlobale.h"

int main(){

    int velocitaInput[] = {50};
    int lunghezzaVelocitaInput = 1;
    int velocitaInCorso= 100;
    //simulazione C.ECU
    
    for(int i = 0; i < lunghezzaVelocitaInput; i++) {
        while(velocitaInCorso > velocitaInput[i]){
            brakebywire(0,&velocitaInCorso);
        }
    }
   


    return 0;
}

