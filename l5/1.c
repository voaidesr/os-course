
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <bits/sigaction.h>

void handle(const int sig) {
    const char *msg = "Caught SIGINT\n";
    write(STDOUT_FILENO, msg, strlen(msg));
}

int main(int argc, char const *argv[])
{
    printf("The pid is: %d \n", getpid());

    struct sigaction sa = { 0 };
    sa.sa_handler = handle;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);

    while (1)
    {
        1 == 1;
    }

    return 0;
}
