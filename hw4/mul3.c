// mul3.c
#include <stdio.h>

// 宣告外部匯編函式 mul3
// 參數：a, b, c 三個整數
// 回傳值：a * b * c 的乘積
int mul3(int a, int b, int c);

int main() {
    // 呼叫 mul3，並把結果印出來
    printf("mul3(3,2,5)=%d\n", mul3(3, 2, 5));
    return 0;
}
