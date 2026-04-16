#include "../common/utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define ASIZE 256
static void preQsBc(char *x, int m, int qsBc[]) {
    int i;
    for (i = 0; i < ASIZE; ++i) qsBc[i] = m + 1;
    for (i = 0; i < m; ++i) qsBc[(unsigned char)x[i]] = m - i;
}
void QS(char *x, int m, char *y, int n, int verbose) {
    int j, qsBc[ASIZE];
    preQsBc(x, m, qsBc);
    j = 0;
    while (j <= n - m) {
        int k;
        for (k = 0; k < m && COMPARE(x[k], y[j + k]); ++k);
        if (k == m) {
            if (verbose) printf("Found at: %d\n", j);
        }
        j += qsBc[(unsigned char)y[j + m]];
    }
}
int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Usage: %s <pattern> <text> <repeat> [--verbose]\n", argv[0]);
        return 1;
    }
    int plen, tlen, repeat = atoi(argv[3]);
    int verbose = (argc >= 5 && strcmp(argv[4], "--verbose") == 0);
    char *pattern = read_file(argv[1], &plen);
    char *text = read_file(argv[2], &tlen);
    if (!pattern || !text) return 1;
    if (verbose) printf("Pattern: %s\nText length: %d\n", pattern, tlen);
    double time_ms; unsigned long long comps;
    for (int r = 0; r < repeat; r++) {
        run_once(QS, pattern, plen, text, tlen, &time_ms, &comps, verbose);
        printf("%f,%llu\n", time_ms, comps);
    }
    free(pattern); free(text);
    return 0;
}