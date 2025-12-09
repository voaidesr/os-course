#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>

#define BUF_SIZE 1024

int error_exit(const char *msg) {
    perror(msg);
    return 1;
}

int main() {
    int sv[2];
    pid_t pid;
    char buffer[BUF_SIZE];
    ssize_t bytes_read;
    int file_fd;

    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sv) == -1) {
        error_exit("Eroare la socketpair");
    }

    pid = fork();
    if (pid == -1) {
        error_exit("Eroare la fork");
    }

    if (pid == 0) {
        close(sv[0]);

        printf("In Child Process: reading message.\n");

        file_fd = open("mesaj_copil.txt", O_RDONLY);
        if (file_fd == -1) error_exit("Child: cannot open messahe.");

        int n = read(file_fd, buffer, BUF_SIZE);
        close(file_fd);

        if (n > 0) {
            write(sv[1], buffer, n);
            printf("In Child Process: message sent.\n");
        }

        printf("In Child Process: waiting for response.\n");
        n = read(sv[1], buffer, BUF_SIZE);
        if (n > 0) {
            buffer[n] = '\0';
            printf("In child process: received message: %s\n", buffer);
        }

        close(sv[1]);
        _exit(0);
    }

    else {
        close(sv[1]);

        printf("Inside Parent: waiting for message from child.\n");

        int n = read(sv[0], buffer, BUF_SIZE);
        if (n > 0) {
            buffer[n] = '\0';
            printf("From parent, received message: %s\n", buffer);
        }

        file_fd = open("mesaj_parinte.txt", O_RDONLY);
        if (file_fd == -1)
            error_exit("In parent: cannot open file.");

        n = read(file_fd, buffer, BUF_SIZE);
        close(file_fd);

        if (n > 0) {
            write(sv[0], buffer, n);
            printf("Inside parent: message sent\n");
        }

        wait(NULL);
        close(sv[0]);
    }

    return 0;
}