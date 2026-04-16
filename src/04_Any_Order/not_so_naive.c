#include "../common/utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
void HSH(char *x, int m, char *y, int n, int verbose) {
    int j, k, ell;
    if (x[0] == x[1]) { k = 2; ell = 1; }
    else { k = 1; ell = 2; }
    j = 0;
    while (j <= n - m) {
        if (!COMPARE(x[1], y[j + 1]))
            j += k;
        else {
            int i;
            for (i = 2; i < m; ++i)
                if (!COMPARE(x[i], y[j + i])) break;
            if (i == m && COMPARE(x[0], y[j])) {
                if (verbose) printf("Found at: %d\n", j);
            }
            j += ell;
        }
    }
}
int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Usage: %s <pattern> <text> <repeat> [--verbose]\n", argv[0]);
        return 1;
    }
    int plen, tlen, repeat = atoi(argv[3]);
    int verbose = (argc >= 5 && strcmp(argv[4], "--verbose") == 0);
    char *pattern = read_file(argv[1], &plen), *text = read_file(argv[2], &tlen);
    if (!pattern || !text) return 1;
    if (verbose) printf("Pattern: %s\nText length: %d\n", pattern, tlen);
    double time_ms; unsigned long long comps;
    for (int r = 0; r < repeat; r++) {
        run_once(HSH, pattern, plen, text, tlen, &time_ms, &comps, verbose);
        printf("%f,%llu\n", time_ms, comps);
    }
    free(pattern); free(text);
    return 0;
}