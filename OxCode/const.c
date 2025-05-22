#include <stdio.h>

int main() {
    int a = 1, b = 2;
    
    // 1. 指向常量的指针
    const int *ptr1 = &a;
    // *ptr1 = 10;      // 错误
    a = 10;             // 合法
    ptr1 = &b;          // 合法

    // 2. 常量指针
    int *const ptr2 = &a;
    *ptr2 = 20;         // 合法
    // ptr2 = &b;       // 错误

    // 3. 指向常量的常量指针
    const int *const ptr3 = &a;
    // *ptr3 = 30;      // 错误
    // ptr3 = &b;       // 错误
    a = 30;             // 合法

    return 0;
}