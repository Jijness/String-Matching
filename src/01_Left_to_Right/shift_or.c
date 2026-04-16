#include "../common/utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#define WORD (sizeof(unsigned int) * CHAR_BIT)

static unsigned int S[256];
static void preSo(char *x, int m) {
    unsigned int j, lim;
    int i;
    for (i = 0; i < 256; ++i) S[i] = ~0U;
    for (lim = i = 0, j = 1; i < m; ++i, j <<= 1) {
        S[(unsigned char)x[i]] &= ~j;
        lim |= j;
    }
    lim = ~(lim >> 1);
}
void SO(char *x, int m, char *y, int n, int verbose) {
    if (m > WORD) { fprintf(stderr, "Pattern too long\n"); return; }
    preSo(x, m);
    unsigned int state = ~0U, lim = (1U << (m - 1)) - 1;
    for (int j = 0; j < n; ++j) {
        state = (state << 1) | S[(unsigned char)y[j]];
        if ((state & (1U << (m - 1))) == 0) {
            if (verbose) printf("Found at: %d\n", j - m + 1);
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
        run_once(SO, pattern, plen, text, tlen, &time_ms, &comps, verbose);
        printf("%f,%llu\n", time_ms, comps);
    }
    free(pattern); free(text);
    return 0;
}