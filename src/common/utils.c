#include "utils.h"

unsigned long long comparisons = 0;

void reset_comparisons() {
    comparisons = 0;
}

void run_once(SearchFunc func, char *pattern, int m, char *text, int n,
              double *time_ms, unsigned long long *comps) {
    clock_t start, end;
    reset_comparisons();
    start = clock();
    func(pattern, m, text, n);
    end = clock();
    *time_ms = ((double)(end - start) / CLOCKS_PER_SEC) * 1000.0;
    *comps = comparisons;
}

char* read_file(const char *filename, int *length) {
    FILE *f = fopen(filename, "rb");
    if (!f) return NULL;
    fseek(f, 0, SEEK_END);
    *length = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *buf = (char*)malloc(*length + 1);
    if (!buf) {
        fclose(f);
        return NULL;
    }
    fread(buf, 1, *length, f);
    buf[*length] = '\0';
    fclose(f);
    return buf;
}