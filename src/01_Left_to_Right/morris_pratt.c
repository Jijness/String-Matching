#include "../common/utils.h"
#include <stdlib.h>
#include <stdio.h>

// Tiền xử lý mảng mpNext
void preMp(char *x, int m, int *mpNext) {
    int i = 0, j = -1;
    mpNext[0] = -1;
    while (i < m) {
        while (j > -1 && !COMPARE(x[i], x[j]))
            j = mpNext[j];
        i++;
        j++;
        mpNext[i] = j;
    }
}

// Hàm tìm kiếm chính
void MP(char *x, int m, char *y, int n) {
    int i, j, *mpNext;
    mpNext = (int*)malloc((m + 1) * sizeof(int));
    if (!mpNext) return;

    /* Preprocessing */
    preMp(x, m, mpNext);

    /* Searching */
    i = j = 0;
    while (j < n) {
        while (i > -1 && !COMPARE(x[i], y[j]))
            i = mpNext[i];
        i++;
        j++;
        if (i >= m) {
            // OUTPUT(j - i);  // Có thể in ra vị trí tìm thấy nếu cần
            i = mpNext[i];
        }
    }

    free(mpNext);
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
        run_once(MP, pattern, plen, text, tlen, &time_ms, &comps);
        printf("%f,%llu\n", time_ms, comps);
    }

    free(pattern);
    free(text);
    return 0;
}