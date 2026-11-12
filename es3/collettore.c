#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

#define FIFO_PATH "/tmp/fifo_pari"

int main(void) {
    if (mkfifo(FIFO_PATH, 0666) == -1) {
        if (errno != EEXIST) {
            perror("mkfifo");
            exit(EXIT_FAILURE);
        }
    }

    printf("[Collettore] In attesa di dati...\n");
    fflush(stdout);

    int fdFIFO = open(FIFO_PATH, O_RDONLY);
    if (fdFIFO == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    int x;
    int i=0;
    int somma = 0;

    while (read(fdFIFO, &x, sizeof(x)) == sizeof(x)) {
        if (x == -1) break;
        printf("[Collettore] Ricevuto: %d\n", x);
        i++;
        somma +=x;
    }

    printf("[Collettore] Fine. Numeri pari ricevuti: %d, somma: %d\n", i, somma);

    close(fdFIFO);
    unlink(FIFO_PATH);
    return 0;
}