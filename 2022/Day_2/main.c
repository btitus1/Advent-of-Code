#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define FILENAME "datafile.txt"

int main() {
    char *filename = FILENAME;
    FILE *fp = fopen(filename, "r");

    if (fp == NULL) {
        printf("Error: could not open file %s", filename);
        exit(1);
    }

    char a, b;
    int p1 = 0, p2 = 0;

    char buf[64], *line;
    do {
        // get the line
        line = fgets(buf, sizeof(buf), fp);

        if (line && sscanf(line, "%c %c", &a, &b) == 2) {
            a -= 'A';
            b -= 'X';

            switch ((b + 3 - a) % 3) {
                case 0:
                    p1 += b + 1 + 3;
                    break;
                case 1:
                    p1 += b + 1 + 6;
                    break;
                case 2:
                    p1 += b + 1;
                    break;
                default:
                    printf("Unexpected input : %d\n", (b + 3 - a) % 3);
                    exit(1);
            }

            switch (b) {
                case 0:
                    p2 += (a + 2) % 3 + 1;
                    break;
                case 1:
                    p2 += a + 1 + 3;
                    break;
                case 2:
                    p2 += (a + 1) % 3 + 1 + 6;
                    break;
                default:
                    printf("Unexpected input : %d\n", b);
                    exit(1);
            }
        }
    } while (line);

    printf("\nTotal points based on strategy guide: %d\n", p1);
    printf("Total points based on NEW strategy guide: %d\n", p2);

    return 0;
}