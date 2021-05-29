#include <stdio.h>
#include <stdio.h>
#include <fcntl.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#define BUFSIZE 1024

int main(int argc, char const *argv[])
{
    int fd, n;
    char buf[BUFSIZE];
    FILE *fp_r;
    const char *in_file = argv[1];
    mknod("fifo", S_IFIFO|0666, 0);
    fp_r = fopen("test.txt", "r");
    fd = open("fifo", O_WRONLY);
    while ((n=fread(buf, sizeof(char), BUFSIZE, fp_r))) {
        write(fd, buf, n);
    }
    close(fd);
    fclose(fp_r);
}


