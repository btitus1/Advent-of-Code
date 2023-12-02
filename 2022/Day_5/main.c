#include <stdio.h>
#include <string.h>
#include "stack.c"

#define MAX_LENGTH 256

// GLOBAL VARiABLES
int **crates;
int biggest_stack = 0;

int cols;

int move_count = 0;
int *count_list;
int *from_list;
int *to_list;

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

    // count the number of rows needed 
    char buffer[MAX_LENGTH];

    bool done = false;
    while (fgets(buffer, MAX_LENGTH, fp) && !done) {

        // extract the values from the line
        if (strchr(buffer, '[') == NULL) {
            done = true;
        } else {
            biggest_stack++;
            // calculate number of columns 
            cols = (int) ((strlen(buffer) + 1) / 4);
        }
    }

    rewind(fp);

    // create the crates array & initialize with spaces
    crates = (int **) malloc(sizeof(int *) * biggest_stack);

    for (int i = 0; i < biggest_stack; i++) {
        crates[i] = (int *) malloc(sizeof(int) * cols);
        for (int j = 0; j < cols; j++) {
            crates[i][j] = ' ';
        }
    }


    done = false;
    int r = 0;
    while (fgets(buffer, MAX_LENGTH, fp) && !done) {
        // extract the values from the line
        if (strchr(buffer, '[') == NULL) {
            done = true;
        } else {
            for (int c = 0; c < cols; c++) {
                char item = buffer[(4 * c) + 1];
                if (item != ' ') {
                    crates[r][c] = (int) item;
                }
            }
            r++;
        }
    }
    
    fpos_t position;
    fgetpos(fp,&position);
    
    move_count = 0;
    // count the number of moves
    while (fgets(buffer, MAX_LENGTH, fp)) {
        move_count++;
    }
    
    // rewind to start of moves
    fsetpos(fp,&position);

    // allocate the stacks for the moves
    count_list = (int *) malloc(sizeof(int) * move_count);
    from_list = (int *) malloc(sizeof(int) * move_count);
    to_list = (int *) malloc(sizeof(int) * move_count);

    int i = 0;

    //read and extract the moves list
    while (fgets(buffer, MAX_LENGTH, fp)) {
        if (buffer[0] == 'm') {
            const char delimiters[] = "movefromto \n";
            char *token = strtok(buffer, delimiters);
            count_list[i] = (int) strtol(token, NULL, 10);

            token = strtok(NULL, delimiters);
            from_list[i] = (int) strtol(token, NULL, 10);

            token = strtok(NULL, delimiters);
            to_list[i] = (int) strtol(token, NULL, 10);

            i++;
        }
    }
    
    fclose(fp);

    printf("\n");
}

void top_stacks(Stack **stacks, bool brackets) {
    int val;
    for (int c = 0; c < cols; c++) {
        peek(stacks[c], &val);
        if (brackets)
            printf("[");
        printf("%c", val);
        if (brackets)
            printf("]");
    }
    printf("\n");
}

// process the moves list one crate at a time
void part_one() {

    Stack **stacks;
    
    // allocate space & create the stacks
    stacks = malloc(sizeof(Stack*) * cols);
    for (int i=0; i < cols; i++) {
        stacks[i] = create_stack(biggest_stack * cols);    
    }
    
    // push the items on the stacks
    for (int c = 0; c < cols; c++) {
        bool done = false;
        int r = biggest_stack - 1;
        while(r >=0 && !done) {
            if (crates[r][c] != ' ') {
                push(stacks[c], crates[r][c]);
                r--;
            } else {
                done = true;
            }
        }
    }
    
    // process the moves
    for (int m = 0; m < move_count; m++) {
        int n = count_list[m];
        int f = from_list[m];
        int t = to_list[m];
        for (int count = 0; count < n; count++) {
            int val;
            pop(stacks[f-1],&val);
            push(stacks[t-1], val);
        }
    }

    printf("\tPart One - Top of Stacks : ");
    top_stacks(stacks, false);
    
}

// process the moves list in groups
void part_two() {

    Stack **stacks;

    // allocate space & create the stacks
    stacks = malloc(sizeof(Stack*) * cols);
    for (int i=0; i < cols; i++) {
        stacks[i] = create_stack(biggest_stack * cols);
    }

    // push the items on the stacks
    for (int c = 0; c < cols; c++) {
        bool done = false;
        int r = biggest_stack - 1;
        while(r >=0 && !done) {
            if (crates[r][c] != ' ') {
                push(stacks[c], crates[r][c]);
                r--;
            } else {
                done = true;
            }
        }
    }

    // process the moves
    for (int m = 0; m < move_count; m++) {
        int n = count_list[m];
        int f = from_list[m];
        int t = to_list[m];

        Stack *temp = create_stack(n);
        for (int count = 0; count < n; count++) {
            int val;
            pop(stacks[f - 1], &val);
            push(temp, val);

        }
        for (int count = 0; count < n; count++) {
            int val;
            pop(temp, &val);
            push(stacks[t-1], val);
        }
    }

    printf("\tPart Two - Top of Stacks : ");
    top_stacks(stacks, false);

}


int main(void) {

    printf("Advent of Code 2022 Day 5\n");
    read_file();
    part_one();
    part_two();

    return 0;

}