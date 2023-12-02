#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_LENGTH 256
#define MAX_LINES 2000
#define MAX_PIXELS 240
#define PIXEL_WIDTH 40

char commands[MAX_LINES][MAX_LENGTH];
int values[MAX_LINES];
int signal_strength[MAX_LINES];
char pixel[MAX_PIXELS];

int peek[] = {20, 60, 100, 140, 180, 220};

int command_count = 0;
int clock_cycle = 0;
int regX = 1;

void init_pixels() {
    for (int i = 0; i < MAX_PIXELS; ++i) {
        pixel[i] = '.';
    }
}


void set_pixel(int pos, int cycle) {


    bool in = false;
    for (int i = -1; i <= +1; ++i) {
        if ((pos + i) == (cycle-1) % PIXEL_WIDTH) {
            in = true;
            break;
        }
    }
    if (in) {
        //printf("\tCRT draws pixel in position %d\n\t#\n", cycle-1 % PIXEL_WIDTH);
        pixel[cycle - 1] = '#';
    } else {
        //printf("\tCRT draws pixel in position %d\n\t-\n", cycle-1 % PIXEL_WIDTH);
        pixel[cycle - 1] = '.';
    }

}
void draw_pixels() {

    for (int i = 1; i <= MAX_PIXELS; ++i) {

        if (i % PIXEL_WIDTH == 1) {
            printf("Cycle%4d -> %c", i, pixel[i-1]);
        } else if (i % PIXEL_WIDTH == 0) {
            printf("%c <- Cycle %4d\n", pixel[i-1], i);
        } else {
            printf("%c", pixel[i-1]);
        }
    }

}

void addx(int index, int cycle, int loop) {
    if (loop==1) {
        //printf("\tbegin executing addx %d\n",values[index]);
    }
    set_pixel(regX, cycle);
    signal_strength[cycle] = cycle * regX;
    //printf("\t%d [%d] (addx %d) X : %d <-> Signal Strength : %d\n", cycle, loop, values[index], regX, signal_strength[cycle]);


    if (loop == 2) {
        regX += values[index];
        //printf("\tfinish executing addx %d (Register X is now %d\n\n",values[index], regX );
    }

}

void noop(int cycle) {
    signal_strength[cycle] = cycle * regX;
    //printf("\t%d (noop) X : %d <-> Signal Strength : %d\n", cycle, regX, signal_strength[cycle]);
    set_pixel(regX, cycle);
}

void process() {
    for (int i = 0; i < command_count; ++i) {
        clock_cycle++;
        if (strcmp(commands[i], "noop") == 0) {
            noop(clock_cycle);
        } else {
            addx(i, clock_cycle, 1);
            clock_cycle++;
            addx(i, clock_cycle, 2);
        }
    }
}

void print_values() {
    int total = 0;
    for (int i = 0; i < sizeof(peek) / sizeof(int); ++i) {
        printf("Cycle %d : %d\n", peek[i], signal_strength[peek[i]]);
        total += signal_strength[peek[i]];
    }
    printf("Sum of signal strengths: %d\n", total);
}

void read_file() {
    char *filename = "datafile.txt";
    FILE *fp = fopen(filename, "r");

    if (fp == NULL) {
        printf("Error: could not open file %s", filename);
        exit(1);
    }

    // read the data into lines to get line count
    char buffer[MAX_LENGTH];
    while (fgets(buffer, MAX_LENGTH, fp)) {
        const char delimiters[] = " \n";
        char *token = strtok(buffer, delimiters);

        strcpy(commands[command_count], token);

        if (buffer[0] == 'a') {
            token = strtok(NULL, delimiters);
            values[command_count] = (int) strtol(token, NULL, 10);
        }
        command_count++;
    }

// close the file
    fclose(fp);
}

int main() {
    read_file();
    //print_commands();
    init_pixels();
    process();
    print_values();
    draw_pixels();
    return 0;
}