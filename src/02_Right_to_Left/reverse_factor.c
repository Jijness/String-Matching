#include "../common/utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define MAX_STATES 2048
#define ASIZE 256
typedef struct {
    int length, link, next[ASIZE];
} State;
static State st[MAX_STATES];
static int size, last;
static void sa_init() {
    st[0].length = 0; st[0].link = -1;
    memset(st[0].next, -1, sizeof(st[0].next));
    size = 1; last = 0;
}
static void sa_extend(char c) {
    int cur = size++;
    st[cur].length = st[last].length + 1;
    memset(st[cur].next, -1, sizeof(st[cur].next));
    int p = last;
    while (p != -1 && st[p].next[(unsigned char)c] == -1) {
        st[p].next[(unsigned char)c] = cur;
        p = st[p].link;
    }
    if (p == -1) st[cur].link = 0;
    else {
        int q = st[p].next[(unsigned char)c];
        if (st[p].length + 1 == st[q].length) st[cur].link = q;
        else {
            int clone = size++;
            st[clone].length = st[p].length + 1;
            memcpy(st[clone].next, st[q].next, sizeof(st[q].next));
            st[clone].link = st[q].link;
            while (p != -1 && st[p].next[(unsigned char)c] == q) {
                st[p].next[(unsigned char)c] = clone;
                p = st[p].link;
            }
            st[q].link = st[cur].link = clone;
        }
    }
    last = cur;
}
static void buildSuffixAutomaton(char *x, int m) {
    sa_init();
    for (int i = 0; i < m; ++i) sa_extend(x[i]);
}
void RF(char *x, int m, char *y, int n, int verbose) {
    // Làm việc với mẫu đảo
    char *xR = (char*)malloc(m + 1);
    for (int i = 0; i < m; ++i) xR[i] = x[m - 1 - i];
    xR[m] = '\0';
    buildSuffixAutomaton(xR, m);
    free(xR);
    int j = 0;
    while (j <= n - m) {
        int state = 0, len = 0;
        for (int i = m - 1; i >= 0; --i) {
            int c = (unsigned char)y[j + i];
            if (st[state].next[c] == -1) break;
            state = st[state].next[c];
            ++len;
        }
        if (len == m) {
            if (verbose) printf("Found at: %d\n", j);
            j += m - st[st[state].link].length;
        } else j += m - len;
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
        run_once(RF, pattern, plen, text, tlen, &time_ms, &comps, verbose);
        printf("%f,%llu\n", time_ms, comps);
    }
    free(pattern); free(text);
    return 0;
}