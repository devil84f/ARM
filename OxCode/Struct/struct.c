#include <stdio.h>

// ����1����Ĭ�϶��루���ܲ�����С��
struct Nowcoder_default {
    double d;
    int i;
    char c;
}; // ���� 16 �ֽڣ�8 + 4 + 1 + 3 padding��

// ����2�������ն��루��С����
#pragma pack(1) // ǿ�� 1 �ֽڶ���
struct Nowcoder {
    double d;
    int i;
    char c;
}; // 13 �ֽڣ�8 + 4 + 1��
#pragma pack() // �ָ�Ĭ�϶���

int main() {
    printf("sizeof(Nowcoder_default) = %d\n", sizeof(struct Nowcoder_default)); // ������ 16
    printf("sizeof(Nowcoder) = %d\n", sizeof(struct Nowcoder)); // 13
    return 0;
}