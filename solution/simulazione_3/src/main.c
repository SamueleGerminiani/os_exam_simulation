#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <errno.h>

#include "errExit.h"

// Definizione della union semun (necessaria per semctl in molti sistemi Linux moderni)
union semun {
    int val;                /* Value for SETVAL */
    struct semid_ds *buf;   /* Buffer for IPC_STAT, IPC_SET */
    unsigned short *array;  /* Array for GETALL, SETALL */
    struct seminfo *__buf;  /* Buffer for IPC_INFO (Linux-specific) */
};

// Funzioni helper per operazioni P (Wait/Lock) e V (Signal/Unlock)
void semOp(int semid, unsigned short sem_num, short sem_op) {
    struct sembuf sop;
    sop.sem_num = sem_num;
    sop.sem_op = sem_op;
    sop.sem_flg = 0;

    if (semop(semid, &sop, 1) == -1)
        errExit("semop failed");
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

    // Creazione Memoria Condivisa
    // Dimensione: spazio per un solo intero
    int shmid = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
    if (shmid == -1) errExit("shmget failed");

    // Attach della memoria
    int *count = (int *)shmat(shmid, NULL, 0);
    if (count == (void *)-1) errExit("shmat failed");

    // Inizializzazione valore condiviso
    *count = 0;

    //Creazione di 1 semaforo
    int semid = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);
    if (semid == -1) errExit("semget failed");

    // Inizializzazione del semaforo
    union semun arg;
    arg.val = 1;
    if (semctl(semid, 0, SETVAL, arg) == -1) errExit("semctl SETVAL failed");
    printf("Risorse IPC create. Start...\n");

    //Creazione figlio
    pid_t pid = fork();

    if (pid == -1) {
        errExit("fork failed");
    }
    else if (pid == 0) {
        // --- PROCESSO FIGLIO (DECREMENTA) ---
        for (int i = 0; i < n; i++) {
            // P (Wait/Lock)
            semOp(semid, 0, -1);

            // Sezione Critica
            (*count)++;

            // V (Signal/Unlock)
            semOp(semid, 0, 1);
        }

        // Detach memoria (buona prassi)
        shmdt(count);
        exit(0);
    } else {
        // --- PROCESSO PADRE (INCREMENTA) ---
        for (int i = 0; i < n; i++) {
            // P (Wait/Lock)
            semOp(semid, 0, -1);

            // Sezione Critica
            (*count)--;

            // V (Signal/Unlock)
            semOp(semid, 0, 1);
        }
    }

    //Attesa del figlio
    int status;
    wait(&status); // Attende il figlio

    printf("Processo Figlio terminato.\n");
    printf("Valore finale in memoria: %d\n", *count);

    // Detach memoria
    shmdt(count);

    // Rimozione Memoria Condivisa
    if (shmctl(shmid, IPC_RMID, NULL) == -1)
        errExit("shmctl IPC_RMID failed");

    // Rimozione Semaforo
    if (semctl(semid, 0, IPC_RMID) == -1)
        errExit("semctl IPC_RMID failed");

    printf("Risorse IPC rimosse.\n");

    return 0;
}
