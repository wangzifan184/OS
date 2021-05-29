#include <fcntl.h>
#include <unistd.h>

#define BUFSIZE 1024

int main(int argc, char const *argv[])
{
    /* code */
    const char *in_file = argv[1];
    const char *out_file = argv[2];
    char buf[BUFSIZE];
    int fd_r, fd_w, n;
    fd_r = open(in_file, O_RDONLY);
    fd_w = creat(out_file, S_IWUSR|S_IRUSR);
    fd_w = open(out_file, O_WRONLY);
    while ((n=read(fd_r, buf, BUFSIZE)) > 0)
    {
        write(fd_w, buf, n);
    }
    close(fd_r);
    close(fd_w);
    return 0;
}
