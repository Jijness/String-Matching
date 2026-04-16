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
void TUNEDBM(char *x, int m, char *y, int n, int verbose) {
    int j, k, shift, bmBc[ASIZE];
    preBmBc(x, m, bmBc);
    shift = bmBc[(unsigned char)x[m - 1]];
    bmBc[(unsigned char)x[m - 1]] = 0;

    // Tạo bản sao của văn bản có thêm m ký tự x[m-1] ở cuối (để tránh tràn)
    char *yy = (char*)malloc(n + m);
    memcpy(yy, y, n);
    memset(yy + n, x[m - 1], m);

    j = 0;
    while (j < n) {
        k = bmBc[(unsigned char)yy[j + m - 1]];
        while (k != 0) {
            j += k;
            k = bmBc[(unsigned char)yy[j + m - 1]];
        }
        // Kiểm tra m-1 ký tự còn lại
        int i;
        for (i = 0; i < m - 1; ++i)
            if (!COMPARE(x[i], yy[j + i])) break;
        if (i == m - 1) {
            if (verbose) printf("Found at: %d\n", j);
        }
        j += shift;
    }
    free(yy);
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
        run_once(TUNEDBM, pattern, plen, text, tlen, &time_ms, &comps, verbose);
        printf("%f,%llu\n", time_ms, comps);
    }
    free(pattern); free(text);
    return 0;
}