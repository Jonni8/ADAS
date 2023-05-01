#ifndef metodiUtilizzati.h
#define metodiUtilizzati.h

typedef struct{
    int giorno;
    int mese;
    int anno;
    int ore;
    int minuti;
    int secondi;

} DataOraCorrente;

enum Comando {
    DECELERAZIONE,
    S_ARRESTO
};

void brakebywire(enum Comando comando);
DataOraCorrente conversioneData();
void scriviSulFileConData(char testo[], char nomefileTxt[],DataOraCorrente giornoCorrente);


//IMPLEMENTAZIONE METODI 

//(volendo si può utilizzare uno switch-case)
void brakebywire(enum Comando comando){

    DataOraCorrente giornoCorrente = conversioneData();
    char sFreno[] = "FRENO 5";
    char sArresto[] = "ARRESTO AUTO";

    if(comando == DECELERAZIONE){
        scriviSulFileConData(sFreno,"brake.log.txt",giornoCorrente);
    }else if (comando == S_ARRESTO){
        scriviSulFileConData(sArresto,"brake.log.txt",giornoCorrente);
    }
}

DataOraCorrente conversioneData(){

    DataOraCorrente dataCorrente;

    time_t ora_corrente;
    time(&ora_corrente);

    struct tm *tempo_locale;
    tempo_locale = localtime(&ora_corrente);

    //inizializzazione giorno
    dataCorrente.giorno = tempo_locale->tm_mday;
    dataCorrente.mese = tempo_locale->tm_mon + 1;
    dataCorrente.anno = tempo_locale->tm_year + 1900;

    //inizializzazione orario
    dataCorrente.ore = tempo_locale->tm_hour;
    dataCorrente.minuti = tempo_locale->tm_min;
    dataCorrente.secondi = tempo_locale->tm_sec;

    return dataCorrente;
}

void scriviSulFileConData(char testo[], char nomefileTxt[],DataOraCorrente giornoCorrente){
    FILE *pFile;

    //modalità scrittura
    pFile = fopen(nomefileTxt,"a"); //modalità append aggiunge perché se utilizzo "w" cancella e riscrive

    //il prof non parla dell'utilizzo di fprintf per la scrittura di un file
    //ma solo di fwrite. (possibilià di riscrivere il codice)
    if(pFile != NULL){
        fprintf(pFile,"%2d-%2d-%2d %2d:%2d:%2d %s\n",giornoCorrente.giorno, giornoCorrente.mese, giornoCorrente.anno,giornoCorrente.ore, giornoCorrente.minuti,giornoCorrente.secondi,testo);
        fclose(pFile);
    }else{
        printf("Errore scrittura nel file: puntatore uguale a null");
    }


}


#endif

