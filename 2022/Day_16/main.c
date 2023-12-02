#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>

#define FILENAME "datafile.txt"

#define START_TIMER() \
        clock_t start_time = clock();
#define END_TIMER() \
        double elapsed_time = (double) (clock() - start_time) / CLOCKS_PER_SEC; \
        printf("Done in %f seconds\n", elapsed_time);

static char names[64][3];
static int flows[64], is_open[64], dists[64][64];
static size_t nvalves;

u_long iterations;

int min(int a, int b) { return a < b ? a : b; }

int max(int a, int b) { return a > b ? a : b; }

/* creates if not exists */
static int lookup_name(const char name[3]) {
    size_t i;

    for (i = 0; i < nvalves; i++)
        if (!strcmp(name, names[i]))
            return i;

    assert(i + 1 < 64);
    memcpy(names[nvalves++], name, 3);
    return i;
}

static int max_val(
        size_t pos1, int time1,    /* actor 1 pos and time left */
        size_t pos2, int time2)    /* actor 2 */
{

    size_t i;
    int best = 0, rest;

    // negative cycle check for both
    if (time1 <= 1 && time2 <= 1)
        return 0;

    // negative cycle check for time1
    if (time1 <= 0)
        return time2;

    // negative cycle check for time2
    if (time2 <= 0 )
        return time1;

    for (i = 0; i < nvalves; i++) {

        iterations++;

        if (!flows[i]) continue;
        if (is_open[i]) continue;

        is_open[i] = 1;

        if (time1 >= time2) {
            rest = time1 - dists[pos1][i] - 1;
            best = max(best,
                       flows[i] * rest +
                       max_val(i, rest, pos2, time2));
        }

        /* extra check to prevent identical work */
        if (time2 > time1 || (time1 == time2 && pos1 != pos2)) {
            rest = time2 - dists[pos2][i] - 1;
            best = max(best,
                       flows[i] * rest +
                       max_val(pos1, time1, i, rest));
        }

        is_open[i] = 0;
    }

    return best;
}

int
main() {
    char buf[128], name[5], exits[5][3];
    int p1, p2, n, rate, idx;
    size_t i, j, k;

    memset(dists, 0x0f, sizeof(dists)); /* large numbers */

    FILE *input = fopen(FILENAME, "rt");

    if (input == NULL) {
        printf("Error: could not open file %s", FILENAME);
        exit(1);
    }

    while (fgets(buf, sizeof(buf), input)) {
        n = sscanf(buf, strstr(buf, "tunnels")
                        ? "Valve %2s has flow rate=%d; "
                          "tunnels lead to valves %2s, %2s, %2s, %2s, %2s"
                        : "Valve %2s has flow rate=%d; "
                          "tunnel leads to valve %2s",
                   name, &rate,
                   exits[0], exits[1], exits[2], exits[3], exits[4]);

        idx = lookup_name(name);
        flows[idx] = rate;
        for (i = 2; i < (size_t) n; i++)
            dists[idx][lookup_name(exits[i - 2])] = 1;
    }

    /* Floyd-Warshall algorithm - all-pairs of the shortest paths */
    for (k = 0; k < nvalves; k++) {
        for (i = 0; i < nvalves; i++) {
            for (j = 0; j < nvalves; j++) {
                dists[i][j] = min(dists[i][j], dists[i][k] + dists[k][j]);
            }
        }
    }
    idx = lookup_name("AA");
    START_TIMER();
    iterations = 0;
    p1 = max_val(idx, 30, 0, 0);
    printf("(%lu) Most pressure released by only you minutes: %d\n", iterations, p1);

    iterations = 0;
    printf("Be patient...");
    p2 = max_val(idx, 26, idx, 26);
    printf("(%lu) Most pressure released by you and elephant : %d\n", iterations, p2);
    END_TIMER();
    return 0;
}