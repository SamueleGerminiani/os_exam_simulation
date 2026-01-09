#include <stdlib.h>
#include <stdio.h>
#include <time.h> // Non strettamente necessario qui, ma presente nel template

#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#include "errExit.h"

int main(int argc, char *argv[]) {

    // Controllo degli argomenti da riga di comando
    if (argc != 2) {
        printf("Uso: %s numSottoprocessi\n", argv[0]);
        return 0;
    }

    // Conversione del secondo argomento (argv[1]) in un intero.
    // Questo intero rappresenta il numero di sottoprocessi da creare.
    int n = atoi(argv[1]);
    if (n <= 0) { // Modificato per includere 0 come input non valido
        printf("Il numero di sottoprocessi deve essere > 0!\n");
        return 1;
    }

    pid_t pid;
    for (int i = 0; i < n; ++i) {

        // Calcolo del valore secondo la specifica: V = (i + 1) * 2
        // Nota: i parte da 0, quindi usiamo (i + 1)
        int code = (i + 1) * 2;

        // Generazione del sottoprocesso usando 'fork'
        pid = fork();

        if (pid == -1) {
            printf("figlio %d non creato!\n", i);
        }
        else if (pid == 0) {
            // Nel processo figlio, stampa le informazioni ed esce con il valore calcolato
            printf("Figlio generato (PID: %d). Calcolato: %d\n",
                   getpid(), code);

            // Termina il processo restituendo il valore calcolato al padre
            exit(code);
        }
    }

    int status = 0;
    int total_sum = 0; // Variabile accumulatore per la somma

    // Attesa della terminazione di ogni sottoprocesso creato
    while ((pid = wait(&status)) != -1) {

        // Controllo se il figlio è terminato regolarmente
        if (WIFEXITED(status)) {
            // Estrae il codice di uscita (gli 8 bit meno significativi)
            int child_val = WEXITSTATUS(status);

            printf("Figlio %d terminato con stato %d\n", pid, child_val);

            // Aggregazione: somma il valore restituito al totale
            total_sum += child_val;
        }
    }

    // Stampa del risultato finale aggregato
    printf("Somma Finale Aggregata: %d\n", total_sum);

    return 0;
}
