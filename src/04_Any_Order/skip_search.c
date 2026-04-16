#include "../common/utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
#define ASIZE 256
typedef struct _cell {
    int element;
    struct _cell *next;
} *List;
void SKIP(char *x, int m, char *y, int n, int verbose) {
    List z[ASIZE] = {NULL};
    List ptr;
    int i, j, k;
    /* Preprocessing: xây dựng bucket */
    for (i = 0; i < m; ++i) {
        ptr = (List)malloc(sizeof(struct _cell));
        if (!ptr) { fprintf(stderr, "Memory error\n"); exit(1); }
        ptr->element = i;
        ptr->next = z[(unsigned char)x[i]];
        z[(unsigned char)x[i]] = ptr;
    }
    /* Searching */
    for (j = m - 1; j < n; j += m) {
        for (ptr = z[(unsigned char)y[j]]; ptr != NULL; ptr = ptr->next) {
            int start = j - ptr->element;
            if (start < 0 || start > n - m) continue;
            for (k = 0; k < m && COMPARE(x[k], y[start + k]); ++k);
            if (k == m && verbose) printf("Found at: %d\n", start);
        }
    }
    /* Giải phóng bộ nhớ bucket */
    for (i = 0; i < ASIZE; ++i) {
        ptr = z[i];
        while (ptr) {
            List tmp = ptr;
            ptr = ptr->next;
            free(tmp);
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

    double time_ms;
    unsigned long long comps;
    for (int r = 0; r < repeat; r++) {
        run_once(SKIP, pattern, plen, text, tlen, &time_ms, &comps, verbose);
        printf("%f,%llu\n", time_ms, comps);
    }
    free(pattern); free(text);
    return 0;
}