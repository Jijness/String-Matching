#include "../common/utils.h"
#include <stdlib.h>

// Tiền xử lý mảng kmpNext (tối ưu giống tài liệu)
void preKmp(char *x, int m, int *kmpNext) {
    int i = 0, j = -1;
    kmpNext[0] = -1;
    while (i < m) {
        while (j > -1 && !COMPARE(x[i], x[j]))  // Dùng COMPARE để đếm khi so sánh trên chính pattern
            j = kmpNext[j];
        i++;
        j++;
        // Tối ưu: nếu ký tự tiếp theo giống nhau thì kế thừa giá trị kmpNext
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
            // OUTPUT(j - i);  // Có thể bỏ qua hoặc in ra (không ảnh hưởng phép đếm)
            i = kmpNext[i - 1]; // Lưu ý: kmpNext[i] có thể vượt quá mảng, cần lấy kmpNext[i-1]
        }
    }

    free(kmpNext);
}

// Main để chạy với tham số dòng lệnh
int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Usage: %s <pattern_file> <text_file> <repeat>\n", argv[0]);
        return 1;
    }
    int plen, tlen, repeat = atoi(argv[3]);
    char *pattern = read_file(argv[1], &plen);
    char *text = read_file(argv[2], &tlen);

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