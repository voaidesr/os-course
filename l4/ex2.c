#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdlib.h>
#include <limits.h>

void collatz (int n) {
    printf("%d:", n);
    while (n != 1) {
        printf(" %d", n);
        if (n % 2 == 0)
            n = n / 2;
        else n = 3 * n + 1;
    }
    printf(" 1.\n");
}

int main(int argc, const char *argv[]) {
    if (argc != 2) {
        printf("use: ./exec [int]\n");
        return 1;
    }

    int pid = fork();
    if (pid < 0) {
        perror("fork");
        return errno;
    } else if (pid == 0) {
        int val = atoi(argv[1]);
        collatz(val);
    } else {
        wait(NULL);
        printf("Child %d finished!\n", pid);
    }
}


