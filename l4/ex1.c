#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <errno.h>

int main() {
    int pid = fork();

    if (pid < 0) {
        perror("fork");
        return errno;
    }
    else if (pid == 0) {
        char *argv[] = {"ls", NULL};
        execve("/bin/ls", argv, NULL);

    } else {
        int mypid = getpid();
        printf("Parent pid: %d, Child pid: %d\n", mypid, pid);
        wait(NULL);
        printf("Child %d finished!\n", pid);
    }
    return 0;
}