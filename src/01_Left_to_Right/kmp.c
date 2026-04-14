#include "../common/utils.h"
#include <stdlib.h>
#include <stdio.h>   // Cần cho printf

// Tiền xử lý mảng kmpNext
void preKmp(char *x, int m, int *kmpNext) {
    int i = 0, j = -1;
    kmpNext[0] = -1;
    while (i < m) {
        // Trong tiền xử lý, cũng dùng COMPARE để đếm so sánh (thống nhất)
        while (j > -1 && !COMPARE(x[i], x[j]))
            j = kmpNext[j];
        i++;
        j++;
        if (i < m && x[i] == x[j])
            kmpNext[i] = kmpNext[j];
        else
            kmpNext[i] = j;
    }
}

// Hàm tìm kiếm chính
void KMP(char *x, int m, char *y, int n) {
    int i, j, *kmpNext;
    kmpNext = (int*)malloc(m * sizeof(int));
    if (!kmpNext) return;

    /* Preprocessing */
    preKmp(x, m, kmpNext);

    /* Searching */
    i = j = 0;
    while (j < n) {
        while (i > -1 && !COMPARE(x[i], y[j]))
            i = kmpNext[i];
        i++;
        j++;
        if (i >= m) {
            // Tìm thấy, không in ra gì để tránh ảnh hưởng output
            i = kmpNext[i - 1];
        }
    }
    free(kmpNext);
}

// Hàm main nhận tham số dòng lệnh
int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Usage: %s <pattern_file> <text_file> <repeat>\n", argv[0]);
        return 1;
    }
    int plen, tlen, repeat = atoi(argv[3]);
    char *pattern = read_file(argv[1], &plen);
    char *text = read_file(argv[2], &tlen);
    if (!pattern || !text) {
        printf("Error reading input files.\n");
        free(pattern);
        free(text);
        return 1;
    }

    double time_ms;
    unsigned long long comps;
    for (int r = 0; r < repeat; r++) {
        run_once(KMP, pattern, plen, text, tlen, &time_ms, &comps);
        printf("%f,%llu\n", time_ms, comps);
    }

    free(pattern);
    free(text);
    return 0;
}