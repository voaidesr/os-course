#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdlib.h>

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

int main(int argc, char const *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "use ./exec [int1] [int2] ... [intn]");
        return 1;
    }

    int pid = getpid();
    printf("Starting parent %d\n", pid);

    for (int i = 1; i < argc; i++) {
        int arg = atoi(argv[i]);
        pid = fork();

        if (pid < 0) {
            perror("fork:");
            return 1;
        }

        if (pid ==  0) {
            collatz(arg);
            int ppid = getppid();
            int cpid = getpid();
            printf("Done parent %d Me %d\n", ppid, cpid);

            _exit(0);
        }
    }

    while(1) {
        int status;
        if(wait(&status) > 0) continue;
        else break;
    }

    pid = getpid();
    int ppid = getppid();

    printf("Done parent %d Me %d", ppid, pid);

    return 0;
}
