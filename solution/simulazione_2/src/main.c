#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include "errExit.h"

// Variabile globale per gestire lo stato della velocità.
unsigned int interval = 3;

// Il signal handler che verrà eseguito alla ricezione di SIGINT
void sigHandler(int sig) {
    if (sig == SIGINT) {
        // Inverte la modalità (toggle)
        interval = (interval == 3) ? 1 : 3;

        // Stampa il messaggio di cambio velocita'
        printf("Cambio modalità richiesto!\n");
    }
}

int main(int argc, char *argv[]) {
    // Definisce un set di segnali
    sigset_t mySet;
    // Inizializza mySet per contenere TUTTI i segnali
    sigfillset(&mySet);
    // Rimuove SIGINT da mySet 
    sigdelset(&mySet, SIGINT);
    // Applica la maschera: blocca tutto tranne SIGINT
    if (sigprocmask(SIG_SETMASK, &mySet, NULL) == -1)
        errExit("sigprocmask failed");

    //Attacca il signal handler
    if (signal(SIGINT, sigHandler) == (void *)-1)
        errExit("change signal handler failed");

    while (1) {
        // Determina l'intervallo e la stringa in base alla modalità corrente
        char *modeStr = (interval == 1) ? "Turbo" : "Normale";

        //Stampa messaggio del processo
        printf("[%d] Sto lavorando a velocità %s...\n", getpid(), modeStr);

        // usa la sleep; viene interrotta se arriva un segnale (es. SIGINT).
        sleep(interval);
    }

    return 0;
}
