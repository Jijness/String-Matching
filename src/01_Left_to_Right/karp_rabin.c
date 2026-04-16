#include "../common/utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define REHASH(a, b, h, d) ((((h) - (a) * (d)) << 1) + (b))
void KR(char *x, int m, char *y, int n, int verbose) {
    int d, hx, hy, i, j;
    /* Preprocessing: compute d = 2^(m-1) using left shift */
    for (d = i = 1; i < m; ++i)
        d = (d << 1);
    /* Compute hash of pattern and first window of text */
    hx = hy = 0;
    for (i = 0; i < m; ++i) {
        hx = (hx << 1) + (unsigned char)x[i];
        hy = (hy << 1) + (unsigned char)y[i];
    }
    /* Searching */
    j = 0;
    while (j <= n - m) {
        if (hx == hy) {
            /* Verify character by character to avoid false positives */
            int k;
            for (k = 0; k < m && COMPARE(x[k], y[j + k]); ++k);
            if (k >= m) {
                if (verbose) printf("Found at: %d\n", j);
            }
        }
        ++j;
        if (j <= n - m) {
            hy = REHASH((unsigned char)y[j - 1], (unsigned char)y[j + m - 1], hy, d);
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
        run_once(KR, pattern, plen, text, tlen, &time_ms, &comps, verbose);
        printf("%f,%llu\n", time_ms, comps);
    }
    free(pattern); free(text);
    return 0;
}