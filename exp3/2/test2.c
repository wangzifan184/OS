#include <stdio.h>
#include <stdio.h>
#include <fcntl.h>

// #define BUFSIZE 1024

int main(int argc, char const *argv[])
{
    /* code */
    // clock_t start, end;
    const char *in_file = argv[1];
    const char *out_file = argv[2];
    FILE *fp_r, *fp_w;
    char buf;
    int n, fd_w;
    fp_r = fopen(in_file, "r");
    fd_w = creat(out_file, S_IWUSR|S_IRUSR);
    fp_w = fopen(out_file, "w");
    // start = clock();
    while ((buf=fgetc(fp_r))!=EOF)
    {
        fputc(buf, fp_w);
    }
    // end = clock();
    fclose(fp_r);
    fclose(fp_w);
    // printf("stream I/O call: %f\n", (double)(end-start));
    return 0;
}
