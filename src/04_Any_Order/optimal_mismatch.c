#include "../common/utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define ASIZE 256
#define MAX(a,b) ((a)>(b)?(a):(b))
typedef struct {
    int loc;   // vị trí gốc trong mẫu
    char c;    // ký tự tại vị trí đó
    int freq;  // tần suất (để sắp xếp)
} PatternPos;
static int freq[ASIZE];
// Tính tần suất ký tự từ văn bản
static void computeFreq(char *y, int n) {
    memset(freq, 0, sizeof(freq));
    for (int i = 0; i < n; ++i)
        freq[(unsigned char)y[i]]++;
}
// Hàm so sánh cho qsort: sắp xếp theo tần suất tăng dần (hiếm trước),
// nếu bằng nhau thì vị trí giảm dần (ưu tiên vị trí lớn hơn).
static int cmpFreq(const void *a, const void *b) {
    const PatternPos *pa = (const PatternPos*)a;
    const PatternPos *pb = (const PatternPos*)b;
    if (pa->freq != pb->freq)
        return pa->freq - pb->freq;    // tần suất nhỏ hơn đứng trước
    return pb->loc - pa->loc;     // cùng tần suất -> vị trí lớn hơn đứng trước
}
// Bảng dịch chuyển Quick Search
static void preQsBc(char *x, int m, int qsBc[]) {
    for (int i = 0; i < ASIZE; ++i) qsBc[i] = m + 1;
    for (int i = 0; i < m; ++i)
        qsBc[(unsigned char)x[i]] = m - i;
}
void OM(char *x, int m, char *y, int n, int verbose) {
    PatternPos pat[1024];
    int qsBc[ASIZE];
    computeFreq(y, n);   // lấy tần suất từ văn bản
    for (int i = 0; i < m; ++i) {
        pat[i].loc = i;
        pat[i].c = x[i];
        pat[i].freq = freq[(unsigned char)x[i]];
    }
    // Sắp xếp: ký tự hiếm (tần suất thấp) được kiểm tra trước
    qsort(pat, m, sizeof(PatternPos), cmpFreq);
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
        run_once(OM, pattern, plen, text, tlen, &time_ms, &comps, verbose);
        printf("%f,%llu\n", time_ms, comps);
    }
    free(pattern); free(text);
    return 0;
}