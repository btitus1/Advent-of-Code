#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define LINE_LENGTH 24
#define MAX_MONKEYS 5000
#define FILENAME "datafile.txt"

struct monkey {
    char name[5], op, dep_x;    /* dep_x: depends on 'x'? */
    struct monkey *deps[2], *x;    /* x: cache for depends() */
    int64_t val;
};

static struct monkey monkey_list[MAX_MONKEYS], *root, *human;
size_t number_of_monkeys;

/*
 * This function prints out the monkeys and operations
 */
void print_monkeys() {
    for (int i = 0; i < number_of_monkeys; ++i) {
        printf("Name : %s\n", monkey_list[i].name);
        if (monkey_list[i].deps[0] == NULL) {
            printf("Value : %ld\n\n", monkey_list[i].val);
        } else {
            printf("Operation : %s %c %s\n\n", monkey_list[i].deps[0]->name, monkey_list[i].op,
                   monkey_list[i].deps[1]->name);
        }

    }
}

/*
 * Adds a monkey & it's name to the list of monkeys
 */
static struct monkey *by_name(char name[5]) {
    size_t i;

    for (i = 0; i < number_of_monkeys; i++)
        if (!strcmp(name, monkey_list[i].name))
            return &monkey_list[i];

    strcpy(monkey_list[number_of_monkeys].name, name);
    return &monkey_list[number_of_monkeys++];
}

/*
 * Checks for dependency on monkey that may not have been added yet to list of monkeys
 */
static int depends(struct monkey *m, struct monkey *x) {
    if (m == x) return 1;
    if (m->x == x) return m->dep_x;
    if (m->op == 0) return 0;

    m->x = x;
    if (depends(m->deps[0], x)) {
        m->dep_x = 1;
    } else {
        m->dep_x = depends(m->deps[1], x) != 0;
    }
    return m->dep_x;
}

/*
 * Evaluates the expression
 */
static int64_t eval(struct monkey *m) {
    switch (m->op) {
        case 0:
            return m->val;
        case '+':
            return eval(m->deps[0]) + eval(m->deps[1]);
        case '-':
            return eval(m->deps[0]) - eval(m->deps[1]);
        case '*':
            return eval(m->deps[0]) * eval(m->deps[1]);
        case '/':
            return eval(m->deps[0]) / eval(m->deps[1]);
        default:
            assert(!"bad op");
    }
}

/*
 * solves part two for "what do I need to yell"
 */
static int64_t solve(struct monkey *m, struct monkey *x, int64_t val) {
    struct monkey *d0 = m->deps[0], *d1 = m->deps[1];

    if (m == x)
        return val;

    if (depends(d0, x))    /* x in left operand */
        switch (m->op) {
            case '+':
                return solve(d0, x, val - eval(d1));
            case '-':
                return solve(d0, x, val + eval(d1));
            case '*':
                return solve(d0, x, val / eval(d1));
            case '/':
                return solve(d0, x, val * eval(d1));
        }
    else            /* x in right operand */
        switch (m->op) {
            case '+':
                return solve(d1, x, val - eval(d0));
            case '*':
                return solve(d1, x, val / eval(d0));
            case '-':
                return solve(d1, x, eval(d0) - val);
            case '/':
                return solve(d1, x, eval(d0) / val);
        }
    return 0;
}


/*
 * reads the input file and calls to create/store the monkey data
 */
static void read_input(void) {

    FILE *input = fopen(FILENAME, "rt");

    if (input == NULL) {
        printf("Error: could not open file %s", FILENAME);
        exit(1);
    }
    static char b[LINE_LENGTH];
    struct monkey *m;
    char *s, *nm, d1[5], d2[5], c;

    while ((s = fgets(b, sizeof(b), input))) {
        nm = strsep(&s, ":");
        assert(strlen(nm) == 4);

        m = by_name(nm);
        if (!strcmp(nm, "root")) root = m;
        else if (!strcmp(nm, "humn")) human = m;

        if (sscanf(s, " %ld", &m->val) == 1);
        else if (sscanf(s, " %4s %c %4s", d1, &c, d2) == 3) {
            m->op = c;
            m->deps[0] = by_name(d1);
            m->deps[1] = by_name(d2);
        } else
            assert(!"bad scan");
    }

    assert(root);
    assert(human);
}


int main() {

    int64_t p1, p2;

    read_input();
    // print_monkeys();
    p1 = eval(root);
    root->op = '-';
    p2 = solve(root, human, 0);

    printf("Root yells : %lu\n", p1);
    printf("You need tp yell : %lu \n", p2);
    return 0;
}