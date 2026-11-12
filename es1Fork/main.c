#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    int dati[12] = {3, 7, 2, 9, 4, 6, 8, 1, 5, 10, 2, 7};

    pid_t pid[3];
    int status;
    int tot = 0;


    printf("[Padre] Creo 3 figli\n");

    for (int i=0; i<3; i++) {
        pid[i] = fork();

        if (pid[i] < 0) {
            perror("fork");
            exit(-1);
        }

        if (pid[i] == 0) {
            int somma = 0;

            for (int j=i*4; j < i*4+4;j++) {
                somma += dati[j];
            }

            printf("[Figlio] %d, PID %d] Somma parziale = %d\n", i+1, getpid(), somma);
            exit(somma);
        }
    }

    for (int i=0; i<3; i++) {
        if (waitpid(pid[i], &status, 0) < 0) {
            perror("waitpid");
            exit(-1);
        }

        if (WIFEXITED(status)) {
            int parziale = WEXITSTATUS(status);
            printf("[Padre] Figlio %d terminato con codice %d\n", pid[i], parziale);
            tot += parziale;
        }
    }

    printf("[Padre] Somma totale: %d\n", tot);
    return 0;
}
