#include "../common/utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
typedef struct _cell {
    int element;
    struct _cell *next;
} *List;
static List L[1024]; // Đủ cho mẫu dài tối đa 1023
static int getTransition(char *x, int m, int p, List L[], char c) {
    if (p < m - 1 && COMPARE(x[p + 1], c))
        return p + 1;
    if (p > -1) {
        List cell = L[p];
        while (cell) {
            if (COMPARE(x[cell->element], c))
                return cell->element;
            cell = cell->next;
        }
    }
    return -1;
}
static void setTransition(int p, int q, List L[]) {
    List cell = (List)malloc(sizeof(struct _cell));
    cell->element = q;
    cell->next = L[p];
    L[p] = cell;
}
static int preSimon(char *x, int m, List L[]) {
    int i, k, ell;
    List cell;
    memset(L, 0, (m - 2) * sizeof(List));
    ell = -1;
    for (i = 1; i < m; ++i) {
        k = ell;
        cell = (ell == -1 ? NULL : L[k]);
        ell = -1;
        if (COMPARE(x[i], x[k + 1]))
            ell = k + 1;
        else
            setTransition(i - 1, k + 1, L);
        while (cell) {
            k = cell->element;
            if (COMPARE(x[i], x[k]))
                ell = k;
            else
                setTransition(i - 1, k, L);
            cell = cell->next;
        }
    }
    return ell;
}
void SIMON(char *x, int m, char *y, int n, int verbose) {
    int ell = preSimon(x, m, L);
    int state = -1;
    for (int j = 0; j < n; ++j) {
        state = getTransition(x, m, state, L, y[j]);
        if (state >= m - 1) {
            if (verbose) printf("Found at: %d\n", j - m + 1);
            state = ell;
        }
    }
    // Giải phóng bộ nhớ các danh sách liên kết
    for (int i = 0; i < m - 2; ++i) {
        List cell = L[i];
        while (cell) {
            List tmp = cell;
            cell = cell->next;
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
    double time_ms; unsigned long long comps;
    for (int r = 0; r < repeat; r++) {
        run_once(SIMON, pattern, plen, text, tlen, &time_ms, &comps, verbose);
        printf("%f,%llu\n", time_ms, comps);
    }
    free(pattern); free(text);
    return 0;
}