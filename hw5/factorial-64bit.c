// factorial-64bit
#include <stdio.h>

// 使用 64 位元整數以減少溢位
unsigned long long factorial(unsigned int n) {
    if (n <= 1) return 1ULL;
    return (unsigned long long)n * factorial(n - 1);
}

int main() {
    unsigned int n = 20;  // 20! 約 2.43e18，仍在 64-bit 範圍內
    unsigned long long result = factorial(n);
    printf("factorial(%u) = %llu\n", n, result);
    return 0;
}