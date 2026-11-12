#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {

    int fdPADRE[2];
    int fdFIGLIO[2];

    pipe(fdPADRE);
    pipe(fdFIGLIO);

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(-1);
    }

    if (pid == 0) {
        close(fdPADRE[1]);
        close(fdFIGLIO[0]);

        int n, x;
        int somma = 0;
        int min = 0;
        int max = 0;

        read(fdPADRE[0], &n, sizeof(int));

        for (int i = 0; i < n; i++) {
            read(fdPADRE[0], &x, sizeof(int));
            if (i == 0 || x < min) min = x;
            if (i == 0 || x > min) max = x;
            somma += x;
        }

        printf("[Figlio] Ricevuti %d numeri\n", n);
        fflush(stdout);

        int risultati[3] = {somma, min, max};
        write(fdFIGLIO[1], &risultati[0], sizeof(risultati));

        exit(0);
    }


    close(fdPADRE[0]);
    close(fdFIGLIO[1]);

    int n;

    printf("[Padre] Quanti numeri vuoi inserire?\n");
    scanf("%d", &n);

    int numeri[n];

    for (int i = 0; i < n; i++) {
        printf("Numero %d: ", i + 1);
        scanf("%d", &numeri[i]);
    }

    printf("[Padre] Invio %d numeri al figlio.\n", n);
    fflush(stdout);

    write(fdPADRE[1], &n, sizeof(int));
    write(fdPADRE[1], numeri, sizeof(numeri));

    int risultati[n];

    read(fdFIGLIO[0], risultati, sizeof(risultati));

    printf("[Padre] Somma: %d Minimo: %d, Massimo: %d\n", risultati[0], risultati[1], risultati[2]);

    wait(NULL);
    return 0;
}