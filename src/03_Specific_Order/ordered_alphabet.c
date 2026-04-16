#include "../common/utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))
static void nextMaximalSuffix(char *x, int m, int *i, int *j, int *k, int *p) {
    char a, b;
    while (*j + *k < m) {
        a = x[*i + *k];
        b = x[*j + *k];
        if (COMPARE(a, b)) {
            if (*k == *p) { (*j) += *p; *k = 1; }
            else ++(*k);
        } else if (a > b) {
            (*j) += *k; *k = 1; *p = *j - *i;
        } else {
            *i = *j; ++(*j); *k = *p = 1;
        }
    }
}
void SMOA(char *x, int m, char *y, int n, int verbose) {
    int i, ip, j, jp, k, p;
    ip = -1; i = j = jp = 0; k = p = 1;
    while (j <= n - m) {
        while (i + j < n && i < m && COMPARE(x[i], y[i + j])) ++i;
        if (i == 0) {
            ++j; ip = -1; jp = 0; k = p = 1;
        } else {
            if (i >= m && verbose) printf("Found at: %d\n", j);
            nextMaximalSuffix(y + j, i + 1, &ip, &jp, &k, &p);
            if (ip < 0 || (ip < p && memcmp(y + j, y + j + p, ip + 1) == 0)) {
                j += p; i -= p;
                if (i < 0) i = 0;
                if (jp - ip > p) jp -= p;
                else { ip = -1; jp = 0; k = p = 1; }
            } else {
                j += (MAX(ip + 1, MIN(i - ip - 1, jp + 1)) + 1);
                i = jp = 0; ip = -1; k = p = 1;
            }
        }
    }
}
int main(int argc, char *argv[]) {
    if (argc < 4) { printf("Usage: %s <pattern> <text> <repeat> [--verbose]\n", argv[0]); return 1; }
    int plen, tlen, repeat = atoi(argv[3]);
    int verbose = (argc >= 5 && strcmp(argv[4], "--verbose") == 0);
    char *pattern = read_file(argv[1], &plen), *text = read_file(argv[2], &tlen);
    if (!pattern || !text) return 1;
    if (verbose) printf("Pattern: %s\nText length: %d\n", pattern, tlen);
    double time_ms; unsigned long long comps;
    for (int r = 0; r < repeat; r++) {
        run_once(SMOA, pattern, plen, text, tlen, &time_ms, &comps, verbose);
        printf("%f,%llu\n", time_ms, comps);
    }
    free(pattern); free(text);
    return 0;
}