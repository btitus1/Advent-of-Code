#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define FILENAME "datafile.txt"

int main()
{
    char *filename = FILENAME;
    FILE *fp = fopen(filename, "r");

    if (fp == NULL) {
        printf("Error: could not open file %s", filename);
        exit(1);
    }

    char buf[64], *line;
    int cals, sum=0, top[3];

    // set the values to zero
    memset(top, 0, sizeof(top));

    // loop
    do {
        // get the line
        line = fgets(buf, sizeof(buf), fp);

        // if a valid line amd a number, add it to the sum
        if (line && sscanf(line, "%d", &cals) == 1)
            sum += cals;
        else {
            // add to top of the list
            if (sum > top[0]) {
                // push others down
                top[2] = top[1];
                top[1] = top[0];
                top[0] = sum;
            } else if (sum > top[1]) {
                top[2] = top[1];
                top[1] = sum;
            } else if (sum > top[2])
                top[2] = sum;
            sum = 0;
        }
    } while (line);

    printf("Total Calories that Elf is carrying: %d\n", top[0]);
    printf("Total Calories that top three Elves are carrying: %d\n", top[0]+top[1]+top[2]);

    return 0;
}