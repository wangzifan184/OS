#include <stdio.h>
#include <fcntl.h>

#define BUFSIZE 1024

int main(int argc, char const *argv[])
{
    /* code */
    const char *in_file = argv[1];
    const char *out_file = argv[2];
    FILE *fp_r, *fp_w;
    char buf[BUFSIZE];
    int n, fd_w;
    fp_r = fopen(in_file, "r");
    fd_w = creat(out_file, S_IWUSR|S_IRUSR);
    fp_w = fopen(out_file, "w");
    while ((n=fread(buf, sizeof(char), BUFSIZE, fp_r)) > 0)
    {
        fwrite(buf, sizeof(char), n, fp_w);
    }
    fclose(fp_r);
    fclose(fp_w);
    return 0;
}
