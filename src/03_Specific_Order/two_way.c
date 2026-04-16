#include "../common/utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define MAX(a,b) ((a)>(b)?(a):(b))
/* Tính hậu tố cực đại với thứ tự <= */
static int maxSuf(char *x, int m, int *p) {
    int ms = -1, j = 0, k = *p = 1;
    while (j + k < m) {
        char a = x[j + k], b = x[ms + k];
        if (a < b) { j += k; k = 1; *p = j - ms; }
        else if (a == b) {
            if (k != *p) ++k; else { j += *p; k = 1; }
        } else { ms = j; j = ms + 1; k = *p = 1; }
    }
    return ms;
}
/* Tính hậu tố cực đại với thứ tự >= */
static int maxSufTilde(char *x, int m, int *p) {
    int ms = -1, j = 0, k = *p = 1;
    while (j + k < m) {
        char a = x[j + k], b = x[ms + k];
        if (a > b) { j += k; k = 1; *p = j - ms; }
        else if (a == b) {
            if (k != *p) ++k; else { j += *p; k = 1; }
        } else { ms = j; j = ms + 1; k = *p = 1; }
    }
    return ms;
}
void TW(char *x, int m, char *y, int n, int verbose) {
    int i, j, ell, memory, p, per, q;
    /* Tiền xử lý: tìm phân hoạch tới hạn */
    i = maxSuf(x, m, &p);
    j = maxSufTilde(x, m, &q);
    if (i > j) { ell = i; per = p; } else { ell = j; per = q; }

    /* Tìm kiếm */
    if (memcmp(x, x + per, ell + 1) == 0) {
        j = 0; memory = -1;
        while (j <= n - m) {
            i = MAX(ell, memory) + 1;
            while (i < m && COMPARE(x[i], y[i + j])) ++i;
            if (i >= m) {
                i = ell;
                while (i > memory && COMPARE(x[i], y[i + j])) --i;
                if (i <= memory && verbose) printf("Found at: %d\n", j);
                j += per; memory = m - per - 1;
            } else { j += (i - ell); memory = -1; }
        }
    } else {
        per = MAX(ell + 1, m - ell - 1) + 1;
        j = 0;
        while (j <= n - m) {
            i = ell + 1;
            while (i < m && COMPARE(x[i], y[i + j])) ++i;
            if (i >= m) {
                i = ell;
                while (i >= 0 && COMPARE(x[i], y[i + j])) --i;
                if (i < 0 && verbose) printf("Found at: %d\n", j);
                j += per;
            } else j += (i - ell);
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
        run_once(TW, pattern, plen, text, tlen, &time_ms, &comps, verbose);
        printf("%f,%llu\n", time_ms, comps);
    }
    free(pattern); free(text);
    return 0;
}