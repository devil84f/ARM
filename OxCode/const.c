#include <stdio.h>

int main() {
    int a = 1, b = 2;
    
    // 1. ָ������ָ��
    const int *ptr1 = &a;
    // *ptr1 = 10;      // ����
    a = 10;             // �Ϸ�
    ptr1 = &b;          // �Ϸ�

    // 2. ����ָ��
    int *const ptr2 = &a;
    *ptr2 = 20;         // �Ϸ�
    // ptr2 = &b;       // ����

    // 3. ָ�����ĳ���ָ��
    const int *const ptr3 = &a;
    // *ptr3 = 30;      // ����
    // ptr3 = &b;       // ����
    a = 30;             // �Ϸ�

    return 0;
}