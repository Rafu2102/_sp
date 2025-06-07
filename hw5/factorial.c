// factorial.c
#include <stdio.h>

// 計算 n 的階乘
int factorial(int n) {
    if (n <= 1) {
        return 1;
    }
    return n * factorial(n - 1);
}

int main() {
    int n = 5;
    int result = factorial(n);
    printf("factorial(%d) = %d\n", n, result);
    return 0;
}