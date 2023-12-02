#include <stdio.h>
#include <stdlib.h>

#define FILENAME "datafile.txt"

int main()
{
    int c;
    int64_t acc=0, sum=0;
    char buf[128], *b;

    FILE *input = fopen(FILENAME, "rt");

    if (input == NULL) {
        printf("Error: could not open file %s", FILENAME);
        exit(1);
    }

    while ((c = fgetc(input)) != EOF)
        switch(c) {
            case '=':
                acc *= 5;
                acc +=-2;
                break;
            case '-':
                acc *= 5;
                acc +=-1;
                break;
            case '0':
                acc *= 5;
                acc += 0;
                break;
            case '1':
                acc *= 5;
                acc += 1;
                break;
            case '2':
                acc *= 5;
                acc += 2;
                break;
            default:
                sum += acc;
                printf("%lu\n", acc);
                acc = 0;
                break;
        }

    // last value
    sum += acc;
    printf("%lu\n", acc);

    b = buf + sizeof(buf) -1;
    *b = '\0';

    // process to determine SNAFU
    for (; sum; sum = (sum+2) / 5)
        *--b = "=-012"[(sum+2) %5];

    printf("\nSNAFU number to return: %s\n", b);
    return 0;
}