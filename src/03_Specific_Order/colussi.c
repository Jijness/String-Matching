#include "../common/utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define XSIZE 1024
static int preColussi(char *x, int m, int h[], int next[], int shift[]) {
    int i, k, nd, q, r, s;
    int hmax[XSIZE], kmin[XSIZE], nhd0[XSIZE], rmin[XSIZE];
    /* Computation of hmax */
    i = k = 1;
    do {
        while (i < m && x[i] == x[i - k]) i++;
        hmax[k] = i;
        q = k + 1;
        while (hmax[q - k] + k < i) {
            hmax[q] = hmax[q - k] + k;
            q++;
        }
        k = q;
        if (k == i + 1) i = k;
    } while (k <= m);
    /* Computation of kmin */
    memset(kmin, 0, m * sizeof(int));
    for (i = m; i >= 1; --i)
        if (hmax[i] < m) kmin[hmax[i]] = i;
    /* Computation of rmin */
    for (i = m - 1; i >= 0; --i) {
        if (hmax[i + 1] == m) r = i + 1;
        if (kmin[i] == 0) rmin[i] = r;
        else rmin[i] = 0;
    }
    /* Computation of h */
    s = -1; r = m;
    for (i = 0; i < m; ++i)
        if (kmin[i] == 0) h[--r] = i;
        else h[++s] = i;
    nd = s;
    /* Computation of shift */
    for (i = 0; i <= nd; ++i) shift[i] = kmin[h[i]];
    for (i = nd + 1; i < m; ++i) shift[i] = rmin[h[i]];
    shift[m] = rmin[0];
    /* Computation of nhd0 */
    s = 0;
    for (i = 0; i < m; ++i) {
        nhd0[i] = s;
        if (kmin[i] > 0) ++s;
    }
    /* Computation of next */
    for (i = 0; i <= nd; ++i) next[i] = nhd0[h[i] - kmin[h[i]]];
    for (i = nd + 1; i < m; ++i) next[i] = nhd0[m - rmin[h[i]]];
    next[m] = nhd0[m - rmin[h[m - 1]]];
    return nd;
}

void COLUSSI(char *x, int m, char *y, int n, int verbose) {
    int i, j, last, nd, h[XSIZE], next[XSIZE], shift[XSIZE];
    /* Preprocessing */
    nd = preColussi(x, m, h, next, shift);
    /* Searching */
    i = j = 0; last = -1;
    while (j <= n - m) {
        while (i < m && last < j + h[i] && COMPARE(x[h[i]], y[j + h[i]])) i++;
        if (i >= m || last >= j + h[i]) {
            if (verbose) printf("Found at: %d\n", j);
            i = m;
        }
        if (i > nd) last = j + m - 1;
        j += shift[i]; i = next[i];
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
        run_once(COLUSSI, pattern, plen, text, tlen, &time_ms, &comps, verbose);
        printf("%f,%llu\n", time_ms, comps);
    }
    free(pattern); free(text);
    return 0;
}