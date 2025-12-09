#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>

typedef struct
{
    pid_t   pid;
    int     n;
    int     len;
    int     seq[2048];
} collatz_t;

int next_collatz(int x)
{
    if (x == 1) return x;
    else if (x % 2 == 0) return x / 2;
    else return 3 * x + 1;
}

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Use: ./exec [int1] [int2] ... [intn]\n");
        return 1;
    }

    int N = argc - 1;
    size_t shm_size = (size_t) N * sizeof(collatz_t);
    const char *shm_name = "collatz_mem";

    int fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);

    if (fd < 0)
    {
        perror("shm_open");
        return 1;
    }

    if (ftruncate(fd, shm_size) < 0)
    {
        perror("ftruncate");
        return 1;
    }

    collatz_t *entr = mmap(0, shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (entr == MAP_FAILED)
    {
        perror("mmap");
        return 1;
    }

    printf("Starting parent %d\n", getpid());

    for (int i = 0; i < N; i++)
    {
        entr[i].pid = 0;
        entr[i].n   = atoi(argv[i+1]);
        entr[i].len = 0;

        pid_t pid = fork();
        if (pid < 0)
        {
            perror("fork");
            return 1;
        }
        if (pid == 0)
        {
            int n = entr[i].n;
            entr[i].pid = getpid();
            int k = 0;

            while (1)
            {
                entr[i].seq[k++] = n;
                if (n == 1) break;
                n = next_collatz(n);
            }

            entr[i].len = k;

            printf("Child %d finished, Parent %d\n", getpid(), getppid());

            _exit(0);
        }
    }

    int status;
    while (wait(&status) > 0)
    {
        (void)status; // noop
    }

    for (int i = 0; i < N; i++) {
        collatz_t *s = &entr[i];
        printf("%d: ", s->n);
        for (int j = 0; j < s->len; j++) {
            printf("%d%s", s->seq[j], j + 1 == s->len ? "" : " ");
        }
        printf("\n");
    }

    munmap(entr, shm_size);
    shm_unlink(shm_name);

    printf("Child %d finished, Parent %d\n", getpid(), getppid());

    return 0;
}

