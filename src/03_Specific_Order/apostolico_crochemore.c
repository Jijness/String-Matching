#include "../common/utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define XSIZE 1024
#define MAX(a,b) ((a)>(b)?(a):(b))
static void preKmp(char *x, int m, int kmpNext[]) {
    int i = 0, j = -1;
    kmpNext[0] = -1;
    while (i < m) {
        while (j > -1 && !COMPARE(x[i], x[j])) j = kmpNext[j];
        i++; j++;
        if (i < m && COMPARE(x[i], x[j])) kmpNext[i] = kmpNext[j];
        else kmpNext[i] = j;
    }
}
void AXAMAC(char *x, int m, char *y, int n, int verbose) {
    int i, j, k, ell, kmpNext[XSIZE];
    preKmp(x, m, kmpNext);
    for (ell = 1; ell < m && x[ell - 1] == x[ell]; ell++);
    if (ell == m) ell = 0;

    i = ell; j = k = 0;
    while (j <= n - m) {
        while (i < m && COMPARE(x[i], y[i + j])) ++i;
        if (i >= m) {
            while (k < ell && COMPARE(x[k], y[j + k])) ++k;
            if (k >= ell && verbose) printf("Found at: %d\n", j);
        }
        j += (i - kmpNext[i]);
        if (i == ell) k = MAX(0, k - 1);
        else if (kmpNext[i] <= ell) { k = MAX(0, kmpNext[i]); i = ell; }
        else { k = ell; i = kmpNext[i]; }
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
        run_once(AXAMAC, pattern, plen, text, tlen, &time_ms, &comps, verbose);
        printf("%f,%llu\n", time_ms, comps);
    }
    free(pattern); free(text);
    return 0;
}