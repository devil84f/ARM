#include <stdio.h>
#include <stdbool.h>

// 定时器回调类型
typedef void (*timer_callback_t)(void);

// 模拟定时器结构
struct timer {
    unsigned int interval_ms;
    timer_callback_t callback;
    bool is_active;
};

// 初始化定时器
void timer_init(struct timer *t, unsigned int interval, timer_callback_t cb) {
    t->interval_ms = interval;
    t->callback = cb;
    t->is_active = false;
}

// 启动定时器
void timer_start(struct timer *t) {
    t->is_active = true;
    printf("Timer started with interval %d ms\n", t->interval_ms);
}

// 模拟定时器中断服务例程
void timer_isr(struct timer *t) {
    if (t->is_active) {
        printf("Timer expired! Calling callback...\n");
        t->callback();
    }
}

// 用户定义的回调函数
void user_timer_callback(void) {
    printf("User callback: Timer event handled\n");
}

int main() {
    struct timer my_timer;
    
    // 初始化定时器，设置回调
    timer_init(&my_timer, 1000, user_timer_callback);
    timer_start(&my_timer);
    
    // 模拟定时器到期
    timer_isr(&my_timer);
    
    return 0;
}