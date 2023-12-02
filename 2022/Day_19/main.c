#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#define LEN(a)    (sizeof(a)/sizeof(*(a)))
#define NRES    4
#define FILENAME "datafile.txt"

enum {
    GEO, OBS, CLY, ORE
};
struct st {
    int t, resources[NRES], robots[NRES];
};

static int bps[32][NRES][NRES];
static size_t nbp;

static int min(int a, int b) { return a < b ? a : b; }

static int max(int a, int b) { return a > b ? a : b; }

static int recur(int bp[NRES][NRES], struct st st, int best) {
    struct st st2;
    int i, j, tbuy, maxp;

    /* option 1: do nothing (if we make GEO) */
    best = max(best, st.resources[GEO] + st.t * st.robots[GEO]);

    /* prune time spent looking at pointless t=end decisions */
    if (st.t <= 1)
        return best;

    /*
     * Prune by testing upper limit
     */
    if (st.resources[GEO] + st.t * st.robots[GEO] + st.t * (st.t + 1) / 2 < best)
        return best;

    /* option 2[i]: (save up for and) buy new robot of type i */
    for (i = 0; i < NRES; i++) {
        /* only if we don't already make enough of that */
        if (i != GEO) {
            for (j = 0, maxp = 0; j < NRES; j++)
                maxp = max(maxp, bp[j][i]);
            if (st.robots[i] >= maxp)
                continue;
        }

        /* can buy, and how long? */
        tbuy = 1;
        for (j = 0; j < NRES; j++) {
            if (st.resources[j] >= bp[i][j]) continue;
            if (!st.robots[j]) goto nobuy;
            tbuy = max(tbuy,
                       (bp[i][j] - st.resources[j] +
                        st.robots[j] - 1) / st.robots[j] + 1);
            if (tbuy >= st.t) goto nobuy;
        }

        /* skip ahead and buy */
        st2 = st;
        st2.t -= tbuy;
        for (j = 0; j < NRES; j++) {
            st2.resources[j] += st2.robots[j] * tbuy;
            st2.resources[j] -= bp[i][j];
        }
        st2.robots[i]++;

        best = max(best, recur(bp, st2, best));
        nobuy:;
    }

    return best;
}

int
main() {

    FILE *input = fopen(FILENAME, "rt");

    if (input == NULL) {
        printf("Error: could not open file %s", FILENAME);
        exit(1);
    }

    struct st st;
    int i, p1 = 0, p2 = 1;

    while (6 == fscanf(input,
                       " Blueprint %*d:"
                       " Each ore robot costs %d ore."
                       " Each clay robot costs %d ore."
                       " Each obsidian robot costs %d ore and %d clay."
                       " Each geode robot costs %d ore and %d obsidian.",
                       &bps[nbp][ORE][ORE], &bps[nbp][CLY][ORE],
                       &bps[nbp][OBS][ORE], &bps[nbp][OBS][CLY],
                       &bps[nbp][GEO][ORE],
                       &bps[nbp][GEO][OBS])) {
        nbp++;
        assert(nbp < LEN(bps));
    }

    memset(&st, 0, sizeof(st));
    st.robots[ORE] = 1;
    st.t = 24;

    for (i = 0; i < (int) nbp; i++)
        p1 += (i + 1) * recur(bps[i], st, 0);

    st.t = 32;
    for (i = 0; i < min((int) nbp, 3); i++)
        p2 *= recur(bps[i], st, 0);

    printf("Sum of quality level of all blueprints : %d\n", p1);
    printf("Multiplication of three largest geodes : %d \n", p2);
    return 0;
}