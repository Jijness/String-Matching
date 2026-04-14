#include "utils.h"

unsigned long long comparisons = 0;

void reset_comparisons() { comparisons = 0; }

void run_once(SearchFunc func, char *pattern, int m, char *text, int n,
              double *time_ms, unsigned long long *comps) {
    clock_t start = clock();
    reset_comparisons();
    func(pattern, m, text, n);
    clock_t end = clock();
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
    fread(buf, 1, *length, f);
    buf[*length] = '\0';
    fclose(f);
    return buf;
}