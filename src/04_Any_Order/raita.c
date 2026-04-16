#include "../common/utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define ASIZE 256
static void preBmBc(char *x, int m, int bmBc[]) {
    int i;
    for (i = 0; i < ASIZE; ++i) bmBc[i] = m;
    for (i = 0; i < m - 1; ++i) bmBc[(unsigned char)x[i]] = m - i - 1;
}
void RAITA(char *x, int m, char *y, int n, int verbose) {
    int j, bmBc[ASIZE];
    char firstCh = x[0], middleCh = x[m/2], lastCh = x[m-1];

    preBmBc(x, m, bmBc);

    j = 0;
    while (j <= n - m) {
        char c = y[j + m - 1];
        if (COMPARE(lastCh, c) &&
            COMPARE(firstCh, y[j]) &&
            COMPARE(middleCh, y[j + m/2])) {
            int k;
            for (k = 1; k < m - 1; ++k)
                if (!COMPARE(x[k], y[j + k])) break;
            if (k == m - 1 && verbose) printf("Found at: %d\n", j);
        }
        j += bmBc[(unsigned char)c];
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
        run_once(RAITA, pattern, plen, text, tlen, &time_ms, &comps, verbose);
        printf("%f,%llu\n", time_ms, comps);
    }
    free(pattern); free(text);
    return 0;
}