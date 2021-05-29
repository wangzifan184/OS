#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>

#define BUFSIZE 1024

int main(int argc, char const *argv[])
{
    /* code */
    clock_t start, end;
    const char *in_file = argv[1];
    const char *out_file = argv[2];
    char buf[BUFSIZE];
    int fd_r, fd_w, n;
    fd_r = open(in_file, O_RDONLY);
    fd_w = creat(out_file, S_IWUSR|S_IRUSR);
    fd_w = open(out_file, O_WRONLY);
    start = clock();
    while ((n=read(fd_r, buf, 1024)) > 0)
    {
        write(fd_w, buf, n);
    }
    end = clock();
    close(fd_r);
    close(fd_w);
    printf("system call: %f\n", (double)(end-start));
    return 0;
}
