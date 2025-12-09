#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Use: ./mycp [src] [des]\n");
        return 1;
    }

    const char *src = argv[1], *dest = argv[2];

    int in = open(src, O_RDONLY);
    if(in < 0)
    {
        perror("open src");
        return 1;
    }

    struct stat st;
    if (fstat(in, &st) < 0)
    {
        perror("fstat");
        close(in);
        return 1;
    }

    int out = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (out < 0)
    {
        perror("open dst");
        close(out);
        return 1;
    }

    char buf[64 * 1024];

    while(1)
    {
        ssize_t r = read(in, buf, sizeof buf);
        if (r == 0) break;
        if (r < 0)
        {
            perror("read");
            return 1;
        }

        ssize_t off = 0;
        while (off < r)
        {
            ssize_t w = write(out, buf + off, (size_t) (r - off));
            if (w < 0)
            {
                perror("write");
                return 1;
            }
            off += w;
        }
    }

    close(in);
    close(out);

    return 0;
}
