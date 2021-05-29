#include <stdio.h>
#include <stdio.h>
#include <fcntl.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#define BUFSIZE 1024

int main(int argc, char const *argv[])
{
    int fd, fd_w, n;
    char buf[BUFSIZE];
    FILE *fp_w;
    const char *out_file = argv[1];
    fd_w = creat(out_file, S_IWUSR|S_IRUSR);
    fp_w = fopen(out_file, "w");
    fd = open("fifo", O_RDONLY);
    while ((n=read(fd, buf, sizeof(buf)))) {
        fwrite(buf, sizeof(char), n, fp_w);
    }
    close(fd);
    fclose(fp_w);
}


