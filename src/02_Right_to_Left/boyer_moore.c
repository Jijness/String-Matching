#include "../common/utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define ASIZE 256
#define XSIZE 1024
static void preBmBc(char *x, int m, int bmBc[]) {
    int i;
    for (i = 0; i < ASIZE; ++i) bmBc[i] = m;
    for (i = 0; i < m - 1; ++i) bmBc[(unsigned char)x[i]] = m - i - 1;
}
static void suffixes(char *x, int m, int *suff) {
    int f, g, i;
    suff[m - 1] = m;
    g = m - 1;
    for (i = m - 2; i >= 0; --i) {
        if (i > g && suff[i + m - 1 - f] < i - g)
            suff[i] = suff[i + m - 1 - f];
        else {
            if (i < g) g = i;
            f = i;
            while (g >= 0 && x[g] == x[g + m - 1 - f]) --g;
            suff[i] = f - g;
        }
    }
}
static void preBmGs(char *x, int m, int bmGs[]) {
    int i, j, suff[XSIZE];
    suffixes(x, m, suff);
    for (i = 0; i < m; ++i) bmGs[i] = m;
    j = 0;
    for (i = m - 1; i >= -1; --i)
        if (i == -1 || suff[i] == i + 1)
            for (; j < m - 1 - i; ++j)
                if (bmGs[j] == m) bmGs[j] = m - 1 - i;
    for (i = 0; i <= m - 2; ++i)
        bmGs[m - 1 - suff[i]] = m - 1 - i;
}
void BM(char *x, int m, char *y, int n, int verbose) {
    int i, j, bmGs[XSIZE], bmBc[ASIZE];
    preBmGs(x, m, bmGs);
    preBmBc(x, m, bmBc);
    j = 0;
    while (j <= n - m) {
        for (i = m - 1; i >= 0 && COMPARE(x[i], y[i + j]); --i);
        if (i < 0) {
            if (verbose) printf("Found at: %d\n", j);
            j += bmGs[0];
        } else {
            j += (bmGs[i] > bmBc[(unsigned char)y[i + j]] - m + 1 + i)
                 ? bmGs[i] : bmBc[(unsigned char)y[i + j]] - m + 1 + i;
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
    char *pattern = read_file(argv[1], &plen);
    char *text = read_file(argv[2], &tlen);
    if (!pattern || !text) return 1;
    if (verbose) printf("Pattern: %s\nText length: %d\n", pattern, tlen);
    double time_ms; unsigned long long comps;
    for (int r = 0; r < repeat; r++) {
        run_once(BM, pattern, plen, text, tlen, &time_ms, &comps, verbose);
        printf("%f,%llu\n", time_ms, comps);
    }
    free(pattern); free(text);
    return 0;
}