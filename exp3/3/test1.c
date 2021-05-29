#include <stdio.h>
#include <stdio.h>
#include <fcntl.h>
#include<unistd.h>

#define BUFSIZE 1024

int main(int argc, char const *argv[])
{
    /* code */
    int chan[2];
    const char *in_file = argv[1];
    const char *out_file = argv[2];
    FILE *fp_r, *fp_w;
    int fd_w;
    

    pipe(chan);
    if (fork())
    {
        close(chan[0]);
        fp_r = fopen(in_file, "r");
        int n;
        char buf[BUFSIZE];
        while ((n=fread(buf, sizeof(char), BUFSIZE, fp_r))) {
            write(chan[1], buf, n);
        }
        close(chan[1]);
        fclose(fp_r);
    }
    else {
        close(chan[1]);
        fd_w = creat(out_file, S_IWUSR|S_IRUSR);
        fp_w = fopen(out_file, "w");
        int n;
        char buf[BUFSIZE];
        while ((n=read(chan[0], buf, BUFSIZE)))
        {
            fwrite(buf, sizeof(char), n, fp_w);
        }
        close(chan[0]);
        fclose(fp_w);
    }
    
}


