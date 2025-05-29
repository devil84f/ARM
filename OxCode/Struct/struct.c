#include <stdio.h>

// 方法1：按默认对齐（可能不是最小）
struct Nowcoder_default {
    double d;
    int i;
    char c;
}; // 可能 16 字节（8 + 4 + 1 + 3 padding）

// 方法2：按紧凑对齐（最小化）
#pragma pack(1) // 强制 1 字节对齐
struct Nowcoder {
    double d;
    int i;
    char c;
}; // 13 字节（8 + 4 + 1）
#pragma pack() // 恢复默认对齐

int main() {
    printf("sizeof(Nowcoder_default) = %d\n", sizeof(struct Nowcoder_default)); // 可能是 16
    printf("sizeof(Nowcoder) = %d\n", sizeof(struct Nowcoder)); // 13
    return 0;
}