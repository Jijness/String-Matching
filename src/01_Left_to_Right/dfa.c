#include "../common/utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define ASIZE 256
static int delta[1024][ASIZE]; // Đủ cho mẫu dài tối đa 1023
void buildDFA(char *x, int m) {
    int i, j, k;
    // Khởi tạo trạng thái 0
    for (j = 0; j < ASIZE; ++j) delta[0][j] = 0;
    delta[0][(unsigned char)x[0]] = 1;

    int prev = 0;
    for (i = 1; i <= m; ++i) {
        // Sao chép transitions từ trạng thái prev
        for (j = 0; j < ASIZE; ++j)
            delta[i][j] = delta[prev][j];
        if (i < m) {
            delta[i][(unsigned char)x[i]] = i + 1;
            prev = delta[prev][(unsigned char)x[i]];
        }
    }
}
void AUT(char *x, int m, char *y, int n, int verbose) {
    buildDFA(x, m);
    int state = 0;
    for (int j = 0; j < n; ++j) {
        state = delta[state][(unsigned char)y[j]];
        if (state == m) {
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
        run_once(AUT, pattern, plen, text, tlen, &time_ms, &comps, verbose);
        printf("%f,%llu\n", time_ms, comps);
    }
    free(pattern); free(text);
    return 0;
}