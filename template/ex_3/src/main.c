#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "errExit.h"

// Definizione della union semun
union semun {
  int val;               /* Value for SETVAL */
  struct semid_ds *buf;  /* Buffer for IPC_STAT, IPC_SET */
  unsigned short *array; /* Array for GETALL, SETALL */
  struct seminfo *__buf; /* Buffer for IPC_INFO (Linux-specific) */
};

// Funzioni helper per operazioni P (Wait) e V (Signal)
void semOp(int semid, unsigned short sem_num, short sem_op) {
  struct sembuf sop;
  sop.sem_num = sem_num;
  sop.sem_op = sem_op;
  sop.sem_flg = 0;

  if (semop(semid, &sop, 1) == -1) errExit("semop failed");
}

int main(int argc, char *argv[]) {
  // 1. Controllo Argomenti
  if (argc != 2) {
    printf("Usage: %s numOperations\n", argv[0]);
    return 0;
  }

  int n = atoi(argv[1]);
  if (n <= 0) {
    printf("N must be > 0\n");
    return 1;
  }

  // Creazione Memoria Condivisa; dimensione: spazio per un solo intero
  int shmid;

  // Attach della memoria
  int *count;

  // Inizializzazione valore condiviso

  // Creazione di 1 semaforo
  int semid;

  // Inizializzazione del semaforo
  union semun arg;
  arg.val = 1;
  if (semctl(semid, 0, SETVAL, arg) == -1) errExit("semctl SETVAL failed");
  printf("Risorse IPC create. Start...\n");

  // Creazione figlio
  pid_t pid = fork();

  if (pid == -1) {
    errExit("fork failed");
  } else if (pid == 0) {
    // --- PROCESSO FIGLIO (DECREMENTA) ---
    for (int i = 0; i < n; i++) {
      // P (Wait)

      // Sezione Critica

      // V (Signal)
    }

    // Detach memoria (buona prassi)

    // Termina processo figlio
    exit(0);
  } else {
    // --- PROCESSO PADRE (INCREMENTA) ---
    for (int i = 0; i < n; i++) {
      // P (Wait)

      // Sezione Critica

      // V (Signal)
    }
  }

  // Attesa del figlio
  int status;
  wait(&status);  // Attende il figlio

  printf("Processo Figlio terminato.\n");
  printf("Valore finale in memoria: %d\n", *count);

  // Detach memoria

  // Rimozione Memoria Condivisa

  // Rimozione Semaforo

  printf("Risorse IPC rimosse.\n");

  return 0;
}
