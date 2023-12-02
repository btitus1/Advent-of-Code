/*
 * --- Day 3: Rucksack Reorganization ---
 *
 * --- Part One ---
 * Find the item type that appears in both compartments of each rucksack.
 * What is the sum of the priorities of those item types?
 *
 * --- Part Two ---
 * Find the item type that corresponds to the badges of each three-Elf group.
 * What is the sum of the priorities of those item types?

 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 256
#define SIZE 52

typedef struct rucksack {
    int item_count_a[SIZE];
    int item_count_b[SIZE];
    int item_size;
    char *contents_a;
    char *contents_b;
} rucksack;


rucksack *bag;
int bag_count = 0;

/*
 * return specific value based on character
 * a - z = 1 - 26
 * A - Z = 27 - 52
 */
int ascii_to_index(char ch) {
    if (ch > 96) {
        return ch - 96;
    } else {
        return ch - 38;
    }
}

/*
 * return character based on index
 * a - z = 1 - 26
 * A - Z = 27 - 52
 */
char index_to_ascii(int ch) {
    if (ch < 26) {
        return (char) (ch + 97);
    } else {
        return (char) (ch + 39);
    }
}


void init_rucksack() {
    for (int i = 0; i < bag_count; i++) {
        for (int j = 0; j < SIZE; j++) {
            bag[i].item_count_a[j] = 0;
            bag[i].item_count_b[j] = 0;
        }
    }
}

void destroy_rucksack() {
    for (int i = 0; i < bag_count; i++) {
        free(bag[i].contents_a);
        free(bag[i].contents_b);
    }
    free(bag);
}

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
        bag_count++;
    }

    rewind(fp);

    // allocate the space for each bag
    bag = malloc(sizeof(rucksack) * bag_count);

    // read the data
    int index = 0;

    init_rucksack();

    while (fgets(buffer, MAX_LENGTH, fp)) {
        int item_size = (int) (strlen(buffer) / 2);
        bag[index].contents_a = malloc(item_size * sizeof(char));
        bag[index].contents_b = malloc(item_size * sizeof(char));

        bag[index].item_size = item_size;

        strncpy(bag[index].contents_a, buffer, item_size);
        strncpy(bag[index].contents_b, &buffer[item_size], item_size);

        for (int i = 0; i < bag[index].item_size; i++) {
            int j;
            j = ascii_to_index(bag[index].contents_a[i]) - 1;
            bag[index].item_count_a[j]++;
            j = ascii_to_index(bag[index].contents_b[i]) - 1;
            bag[index].item_count_b[j]++;
        }
        index++;
    }

    // close the file
    fclose(fp);
}

/*
 * Find commonality between two parts of individual rucksack
 * Calculate priority based on the item found
 * Calculate total priority for all duplicate  items
 */
void part_one() {

    printf("Part One - Data\n");
    int total = 0;

    // for each bag
    for (int i = 0; i < bag_count; i++) {

        printf("\t%s : %s\n", bag[i].contents_a, bag[i].contents_b);

        int priority = 0;

        for (int j = 0; j < SIZE; j++) {
            if (bag[i].item_count_a[j] && bag[i].item_count_b[j]) {
                priority = j + 1;
                total += priority;

                printf("\tCommon character : %c\n", index_to_ascii(j));
                break;
            }
        }

        printf("\tPriority    : %d\n\n", priority);
    }
    printf("Part One - Total Sum of Priorities : %d\n\n", total);

}

/*
 * Find commonality between three elf rucksacks
 * Calculate priority based on the item found
 * Calculate total priority for all duplicate items
 */
void part_two() {

    printf("Part Two - Data\n");

    int total = 0;

    // for each three bags
    for (int i = 0; i < bag_count; i+=3) {

        printf("\tGroup %d\n", (i / 3) + 1);
        printf("\t\t%s%s\n", bag[i].contents_a, bag[i].contents_b);
        printf("\t\t%s%s\n", bag[i+1].contents_a, bag[i+1].contents_b);
        printf("\t\t%s%s\n", bag[i+2].contents_a, bag[i+2].contents_b);

        int priority = 0;

        for (int j = 0; j < SIZE; j++) {
            if ((bag[i+0].item_count_a[j] || bag[i+0].item_count_b[j]) &&
                (bag[i+1].item_count_a[j] || bag[i+1].item_count_b[j]) &&
                (bag[i+2].item_count_a[j] || bag[i+2].item_count_b[j]) ) {
                priority = j + 1;
                total += priority;

                printf("\tCommon character : %c\n", index_to_ascii(j));
                break;
            }
        }

        printf("\tPriority    : %d\n\n", priority);
    }
    printf("Part Two - Total Sum of Priorities : %d\n", total);


}

int main(void) {
    printf("Advent of Code 2022 Day 3\n");

    read_file();

    part_one();
    part_two();

    destroy_rucksack();

    return 0;
}