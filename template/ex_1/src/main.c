#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "errExit.h"

int main(int argc, char *argv[]) {
  // Controllo degli argomenti da riga di comando
  if (argc != 2) {
    printf("Uso: %s numSottoprocessi\n", argv[0]);
    return 0;
  }

  // Conversione del secondo argomento (argv[1]) in un intero.
  int n;

  pid_t pid;
  for (int i = 0; i < n; ++i) {
    // Calcolo del valore secondo la specifica: V = (i + 1) * 2

    // Generazione del sottoprocesso usando 'fork'

    if (pid == -1) {
      // Gestione dell'errore in caso di fallimento di fork

    } else if (pid == 0) {
      // Nel processo figlio, stampa le informazioni ed esce con il valore
      // calcolato

      // Termina il processo restituendo il valore calcolato al padre
    }
  }

  int status = 0;
  int total_sum = 0;  // Variabile accumulatore per la somma

  // Attesa della terminazione di ogni sottoprocesso creato
  while () {
    // Controllo se il figlio è terminato regolarmente con WIFEXITED
    if () {
      // Estrae il codice di uscita

      // Stampa il codice di terminazione del figlio

      // Aggregazione: somma il valore restituito al totale
    }
  }

  // Stampa del risultato finale aggregato

  return 0;
}
