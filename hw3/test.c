// test.c
#include <stdio.h>

// 1. 递归 + do…while 测试
int power2(int n) {
    int r;
    r = 1;            // 声明后第一条语句
    do {
        r = r * 2;
        n = n - 1;
    } while (n > 0);
    return r;
}

int main() {
    // All locals must be declared up front, no “int arr[4]” allowed
    int i, res;
    int *arr;

    // 调用 power2
    res = power2(3);
    printf("power2(3) = %d\n", res);

    // 用 malloc 代替数组声明
    arr = malloc(4 * sizeof(int));
    if (!arr) return 1;

    // 用 do…while 给数组赋值
    i = 0;
    do {
        arr[i] = (i + 1) * res;  // 测试指针索引和表达式
        i = i + 1;
    } while (i < 4);

    // 用 while 打印出来
    i = 0;
    while (i < 4) {
        printf("arr[%d] = %d\n", i, arr[i]);
        i = i + 1;
    }

    free(arr);
    return 0;
}
