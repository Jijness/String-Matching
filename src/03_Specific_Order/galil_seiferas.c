#include "../common/utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define K 4
static char *x, *y;
static int m, n;
static int p, p1, p2, q, q1, q2, s;
static void newP1(void);
static void newP2(void);
static void parse(void);
static void search(int verbose);
static void search(int verbose) {
    while (p <= n - m) {
        while (s + q < m && COMPARE(x[s + q], y[p + s + q])) ++q;
        if (q == m - s) {
            int i;
            for (i = 0; i < s && COMPARE(x[i], y[p + i]); ++i);
            if (i >= s && verbose) printf("Found at: %d\n", p);
        }
        if (q == p1 + q1) { p += p1; q -= p1; }
        else { p += (q / K + 1); q = 0; }
    }
}
static void parse(void) {
    while (1) {
        while (x[s + q1] == x[s + p1 + q1]) ++q1;
        while (p1 + q1 >= K * p1) { s += p1; q1 -= p1; }
        p1 += (q1 / K + 1); q1 = 0;
        if (p1 >= p2) break;
    }
    newP1();
}
static void newP2(void) {
    while (x[s + q2] == x[s + p2 + q2] && p2 + q2 < K * p2) ++q2;
    if (p2 + q2 == K * p2) parse();
    else if (s + p2 + q2 == m) search(0);
    else {
        if (q2 == p1 + q1) { p2 += p1; q2 -= p1; }
        else { p2 += (q2 / K + 1); q2 = 0; }
        newP2();
    }
}
static void newP1(void) {
    while (x[s + q1] == x[s + p1 + q1]) ++q1;
    if (p1 + q1 >= K * p1) { p2 = q1; q2 = 0; newP2(); }
    else if (s + p1 + q1 == m) search(0);
    else { p1 += (q1 / K + 1); q1 = 0; newP1(); }
}
void GS(char *argX, int argM, char *argY, int argN, int verbose) {
    x = argX; m = argM; y = argY; n = argN;
    p = q = s = q1 = p2 = q2 = 0;
    p1 = 1;
    newP1();
    // Sau khi tiền xử lý xong, gọi search để bắt đầu tìm kiếm thực sự
    p = 0; q = 0;
    search(verbose);
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
        run_once(GS, pattern, plen, text, tlen, &time_ms, &comps, verbose);
        printf("%f,%llu\n", time_ms, comps);
    }
    free(pattern); free(text);
    return 0;
}