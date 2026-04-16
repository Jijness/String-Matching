#include "../common/utils.h"
#include <stdlib.h>
#include <stdio.h>
void preKmp(char *x, int m, int *kmpNext) {
    int i = 0, j = -1; kmpNext[0] = -1;
    while (i < m) {
        while (j > -1 && !COMPARE(x[i], x[j]))
            j = kmpNext[j];
        i++; j++;
        if (i < m && x[i] == x[j])
            kmpNext[i] = kmpNext[j];
        else kmpNext[i] = j;
    }
}
void KMP(char *x, int m, char *y, int n, int verbose) {
    int i, j, *kmpNext = (int*)malloc(m * sizeof(int));
    if (!kmpNext) return;
    preKmp(x, m, kmpNext);
    i = j = 0;
    while (j < n) {
        while (i > -1 && !COMPARE(x[i], y[j]))
            i = kmpNext[i];
        i++; j++;
        if (i >= m) {
            if (verbose) printf("Found at: %d\n", j - i);
            i = kmpNext[i - 1];
        }
    }
    free(kmpNext);
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
    if (!pattern || !text) { /* xử lý lỗi */ return 1; }

    if (verbose) printf("Pattern: %s\nText length: %d\n", pattern, tlen);
    double time_ms; unsigned long long comps;
    for (int r = 0; r < repeat; r++) {
        run_once(KMP, pattern, plen, text, tlen, &time_ms, &comps, verbose);
        printf("%f,%llu\n", time_ms, comps);
    }
    free(pattern); free(text);
    return 0;
}