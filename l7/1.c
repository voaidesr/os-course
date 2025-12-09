#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <wait.h>

int main(int argc, char const *argv[])
{
    int fd[2];
    pid_t pid;

    if (pipe(fd) < 0)
    {
        perror("pipe");
        return errno;
    }

    pid = fork();
    if (pid < 0) return errno;

    if (pid > 0)
    {
        close(fd[0]);
        int number_list[] = {10, 20, 30, 40, 50};
        write(fd[1], number_list, sizeof(number_list));
        close(fd[1]);
        wait(NULL);
        printf("Parent ended.");
    }
    else
    {
        close(fd[1]);
        int val, status, sum = 0;

        while ((status = read(fd[0], &val, sizeof(int))) > 0)
        {
            sum += val;
        }
        close(fd[0]);
        printf("Child: sum is  %d\n", sum);
        _exit(0);
    }
    return 0;
}
