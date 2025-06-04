#include <stdio.h>

// 回调函数类型定义
typedef void (*callback_t)(int);

// 注册回调的函数
void register_callback(callback_t cb) {
    printf("Registering callback...\n");
    
    // 模拟事件发生
    int event_data = 42;
    printf("Event occurred, calling callback...\n");
    
    // 调用回调函数
    cb(event_data);
}

// 实际的回调函数实现
void my_callback(int data) {
    printf("Callback called with data: %d\n", data);
}

int main() {
    // 注册回调函数
    register_callback(my_callback);
    return 0;
}