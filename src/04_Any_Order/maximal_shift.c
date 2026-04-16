#include "../common/utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define ASIZE 256
#define MAX(a,b) ((a)>(b)?(a):(b))
typedef struct {
    int loc;        // vị trí gốc trong mẫu
    char c;         // ký tự tại vị trí đó
    int minShift;   // giá trị minShift
} PatternPos;
// Tính minShift cho từng vị trí trong mẫu
static void computeMinShift(char *x, int m, int minShift[]) {
    for (int i = 0; i < m; ++i) {
        int j;
        for (j = i - 1; j >= 0; --j)
            if (x[i] == x[j]) break;
        minShift[i] = (j >= 0) ? i - j : i + 1;  // nếu không tìm thấy trước đó, dùng khoảng cách đến đầu
    }
}
// Hàm so sánh cho qsort: sắp xếp giảm dần theo minShift, nếu bằng nhau thì vị trí lớn hơn đứng trước
static int cmpShift(const void *a, const void *b) {
    const PatternPos *pa = (const PatternPos*)a;
    const PatternPos *pb = (const PatternPos*)b;
    if (pa->minShift != pb->minShift)
        return pb->minShift - pa->minShift;  // giảm dần
    return pb->loc - pa->loc;                // cùng minShift -> vị trí lớn hơn trước
}
// Bảng dịch chuyển Quick Search
static void preQsBc(char *x, int m, int qsBc[]) {
    for (int i = 0; i < ASIZE; ++i) qsBc[i] = m + 1;
    for (int i = 0; i < m; ++i)
        qsBc[(unsigned char)x[i]] = m - i;
}
void MS(char *x, int m, char *y, int n, int verbose) {
    PatternPos pat[1024];
    int qsBc[ASIZE], minShift[1024];
    computeMinShift(x, m, minShift);
    for (int i = 0; i < m; ++i) {
        pat[i].loc = i;
        pat[i].c = x[i];
        pat[i].minShift = minShift[i];
    }
    qsort(pat, m, sizeof(PatternPos), cmpShift);
    preQsBc(x, m, qsBc);
    int j = 0;
    while (j <= n - m) {
        int i;
        for (i = 0; i < m; ++i) {
            if (!COMPARE(pat[i].c, y[j + pat[i].loc]))
                break;
        }
        if (i == m && verbose)
            printf("Found at: %d\n", j);
        j += qsBc[(unsigned char)y[j + m]];
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
        run_once(MS, pattern, plen, text, tlen, &time_ms, &comps, verbose);
        printf("%f,%llu\n", time_ms, comps);
    }
    free(pattern); free(text);
    return 0;
}