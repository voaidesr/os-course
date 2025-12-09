#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <bits/sigaction.h>
#include <sys/wait.h>

const int N = 30;

void handler(const int sig)
{
    while (waitpid(-1, NULL, WNOHANG) > 0)
    {
        1 == 1;
    }
}

int main(int argc, char const *argv[])
{
    struct sigaction sa = {};
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGCHLD, &sa, NULL);

    for (int i = 0; i < N; i++)
    {
        int pid = fork();

        if (pid < 0)
        {
            perror("fork");
            return 1;
        }

        if (pid == 0)
        {
            printf("Finished child: %d, with parent %d\n", getpid(), getppid());
            sleep(1);
            _exit(0);
        }
    }

    sleep(3);
    printf("Finished process %d, ended\n", getpid());

    return 0;
}


