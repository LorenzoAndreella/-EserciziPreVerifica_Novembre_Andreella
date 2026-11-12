#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

#define FIFO_PATH "/tmp/fifo_pari"
#define TOT 20

int main(void) {
    int fd[2];

    pipe(fd);

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(-1);
    }

    if  (pid == 0) {
        close(fd[0]);

        srand(time(NULL) ^ getpid());
        for (int i = 0; i < TOT; i++) {
            int x = rand() % 100 + 1;
            write(fd[1], &x, sizeof(x));
        }

        close(fd[1]);
        exit(0);
    }

    close(fd[1]);

    if (mkfifo(FIFO_PATH, 0666)  == -1) {
        if (errno != EEXIST) {
            perror("mkfifo");
            exit(EXIT_FAILURE);
        }
    }

    printf("[Filtro] In attesa del collettore...\n");
    int fdFIFO = open(FIFO_PATH, O_WRONLY);
    if (fdFIFO == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    int x;

    while (read(fd[0], &x, sizeof(x)) == sizeof(x)) {
        if (x % 2 == 0) {
            printf("[Filtro] %d pari, inoltro\n", x);
            write(fdFIFO, &x, sizeof(x));
        } else {
            printf("[Filtro] %d dispari, scarto\n", x);
        }
    }

    int uscita = -1;
    write(fdFIFO, &uscita, sizeof(uscita));

    close(fd[0]);
    close(fdFIFO);
    wait(NULL);
    return 0;
}