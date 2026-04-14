#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

// Biến toàn cục đếm số lần so sánh ký tự
extern unsigned long long comparisons;

// Reset bộ đếm
void reset_comparisons();

// Macro so sánh có đếm (dùng trong toàn bộ thuật toán)
#define COMPARE(a, b) (++comparisons, (a) == (b))

// Định nghĩa kiểu con trỏ hàm tìm kiếm
typedef void (*SearchFunc)(char*, int, char*, int);

// Hàm đo thời gian (ms) và số so sánh cho một lần chạy
void run_once(SearchFunc func, char *pattern, int m, char *text, int n,
              double *time_ms, unsigned long long *comps);

// Đọc file vào buffer (nhớ free)
char* read_file(const char *filename, int *length);

#endif