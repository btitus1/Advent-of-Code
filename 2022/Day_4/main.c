/*
 * --- Day 4: Camp Cleanup ---
 *
 * --- Part One ---
 * In how many assignment pairs does one range fully contain the other? (subset)
 *
 * --- Part Two ---
 * In how many assignment pairs do the ranges overlap? (intersection)
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sets.c"

#define MAX_LENGTH 256

// collection of pairs
typedef struct pairs {
    Set *elf1;
    Set *elf2;
} pairs;

// global variables
pairs *elf_list;
int elf_count = 0;

/*
 * Reads in the text data file and parses int elf_list array
 */
void read_file() {
    char *filename = "datafile.txt";
    FILE *fp = fopen(filename, "r");

    if (fp == NULL) {
        printf("Error: could not open file %s", filename);
        exit(1);
    }

    // buffer
    char buffer[MAX_LENGTH];

    // count the lines
    while (fgets(buffer, MAX_LENGTH, fp)) {
        elf_count++;
    }

    // rewind the file to beginning
    rewind(fp);

    // allocate the space for each elf set
    elf_list = (pairs *) malloc(sizeof(pairs) * elf_count);
    for (int i = 0; i < elf_count; i++) {
        elf_list[i].elf1 = init();
        elf_list[i].elf2 = init();
    }

    // read the data
    int index = 0;

    while (fgets(buffer, MAX_LENGTH, fp)) {

        char *ptr;
        char *remainder;

        int low, high;

        // find the first set of low and high values
        ptr = strtok(buffer, "-");
        low = (int) strtol(ptr, &remainder, 10);
        ptr = strtok(NULL, ",");
        high = (int) strtol(ptr, &remainder, 10);

        // insert the data in the elf1 set
        for (int j = low; j <= high; j++) {
            insert(elf_list[index].elf1, j);
        }

        // find the second set of low and high values
        ptr = strtok(NULL, "-");
        low = (int) strtol(ptr, &remainder, 10);
        ptr = strtok(NULL, ",");
        high = (int) strtol(ptr, &remainder, 10);

        // insert the data in the elf2 set
        for (int j = low; j <= high; j++) {
            insert(elf_list[index].elf2, j);
        }

        index++;
    }

    // close the file
    fclose(fp);
}

/*
 * Find where one pair is fully contained within the other
 * This is determining if Set A is a SUBSET of Set B (and visa-versa)
 */
void part_one() {

    int total = 0;

    // for each elf set
    for (int i = 0; i < elf_count; i++) {
        if (is_subset(elf_list[i].elf1, elf_list[i].elf2) || is_subset(elf_list[i].elf2, elf_list[i].elf1)) {
            total++;
        }
    }

    printf("\nPart One - Total fully contained sets: %d\n", total);
}

/*
 * Find where one pair overlaps another
 * This is determining whether an there is an INTERSECTION between Set A & Set B
 */
void part_two() {

    int total = 0;

    // for each elf set
    for (int i = 0; i < elf_count; i++) {
        Set *common = set_intersection(elf_list[i].elf1, elf_list[i].elf2);
        if (common->length != 0) {
            total++;
        }
    }
    printf("Part Two - Total overlapping sets: %d\n", total);
}

void destroy_sets() {
    for (int i = 0; i < elf_count; i++) {
        destroy_set(&elf_list[i].elf1);
        destroy_set(&elf_list[i].elf2);
    } 
    
    free(elf_list);
}
int main() {
    printf("Advent of Code 2022 Day 4\n");
    read_file();
    part_one();
    part_two();
    
    destroy_sets();
    return 0;
}