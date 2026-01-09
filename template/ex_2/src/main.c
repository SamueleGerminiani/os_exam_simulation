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

  // Inizializza mySet per contenere TUTTI i segnali

  // Rimuove SIGINT da mySet

  // Applica la maschera: blocca tutto tranne SIGINT

  // Attacca il signal handler

  while (1) {
    // Determina l'intervallo e la stringa in base alla modalità corrente
    char *modeStr = (interval == 1) ? "Turbo" : "Normale";

    // Stampa messaggio del processo

    // usa la 'sleep'; viene interrotta se arriva un segnale (es. SIGINT).
  }

  return 0;
}
