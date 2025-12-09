#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <signal.h>

#define MAX_BUF 1024

int fd_write, fd_read;

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Utilizare: %s <fifo_scriere> <fifo_citire>\n", argv[0]);
        return 1;
    }

    char *write_fifo = argv[1];
    char *read_fifo = argv[2];
    char buffer[MAX_BUF];

    mkfifo(write_fifo, 0666);
    mkfifo(read_fifo, 0666);

    fd_write = open(write_fifo, O_RDWR);
    if (fd_write == -1) {
        perror("Eroare open write");
        return 1;
    }

    fd_read = open(read_fifo, O_RDWR);
    if (fd_read == -1) {
        perror("Eroare open read");
        return 1;
    }

    pid_t pid = fork();

    if (pid == 0) {
        char recv_buf[MAX_BUF];
        while (1) {
            int bytes = read(fd_read, recv_buf, MAX_BUF);
            if (bytes > 0) {
                recv_buf[bytes] = '\0';
                if (recv_buf[0] != '\n') {
                    printf("Partener: %s", recv_buf);
                    fflush(stdout);
                }
            }
        }
    }
    else {
        while (1) {
            if (fgets(buffer, MAX_BUF, stdin) == NULL) break;

            write(fd_write, buffer, strlen(buffer));
        }

        kill(pid, SIGKILL);
        close(fd_write);
        close(fd_read);
    }

    return 0;
}