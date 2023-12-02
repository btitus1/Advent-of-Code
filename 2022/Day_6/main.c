/*
 * --- Day 6: Tuning Trouble ---
 * 
 * - Part One -
 * How many characters need to be processed before the first start-of-packet marker is detected?
 * 
 *  - Part Two -
 * How many characters need to be processed before the first start-of-message marker is detected?
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "sets.c"

#define MAX_LENGTH 4096
#define MAX_LINES 1024

// GLOBAL VARiABLES
char *input[MAX_LINES];
char buffer[MAX_LENGTH];
int input_count = 0;

/*
 * Read the data from the datafile
 */
void read_file() {
    char *filename = "datafile.txt";
    FILE *fp = fopen(filename, "r");

    if (fp == NULL) {
        printf("Error: could not open file %s", filename);
        exit(1);
    }

    while (fgets(buffer, MAX_LENGTH, fp)) {
        input[input_count] = malloc(sizeof(char) * (strlen(buffer) - 1));
        strncpy(input[input_count], buffer, (strlen(buffer) - 1));
        input_count++;
    }
    fclose(fp);
}

//
//void print_string(char *str, int start) {
//    printf("%*s%s\n%s\n", start, "", "v   v", str);
//}

// 
void analyze(int packet_size) {

    for (int i = 0; i < input_count; i++) {
        int start = 0;
        int found = start + packet_size;

        bool done = false;
        while (!done && (found <= strlen(input[i]))) {

            Set *packet = init();
            for (int j = 0; j < packet_size; j++) {
                insert(packet, input[i][start + j]);
            }
            if (packet->length == packet_size) {
                done = true;
                printf("\t\t%s : %d\n", input[i], start + packet_size);
            } else {
                start++;
                found++;
            }
        }
    }


}

int main(void) {

    printf("Advent of Code 2022 Day 6\n");
    read_file();
    printf("\tPart One - First Start of Packet\n");
    analyze(4);
    printf("\tPart One - First Start of Message\n");
    analyze(14);
    return 0;
}