#include <stdio.h>

// �ص��������Ͷ���
typedef void (*callback_t)(int);

// ע��ص��ĺ���
void register_callback(callback_t cb) {
    printf("Registering callback...\n");
    
    // ģ���¼�����
    int event_data = 42;
    printf("Event occurred, calling callback...\n");
    
    // ���ûص�����
    cb(event_data);
}

// ʵ�ʵĻص�����ʵ��
void my_callback(int data) {
    printf("Callback called with data: %d\n", data);
}

int main() {
    // ע��ص�����
    register_callback(my_callback);
    return 0;
}