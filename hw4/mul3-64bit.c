// mul3.c
#include <stdio.h>

// 宣告外部匯編函式，參數與回傳值都用 64 位 long long
long long mul3(long long a, long long b, long long c);

int main() {
    long long x = 55555, y = 9487666, z = 5201314;
    long long result = mul3(x, y, z);
    printf("mul3(%lld,%lld,%lld)=%lld\n", x, y, z, result);
    return 0;
}