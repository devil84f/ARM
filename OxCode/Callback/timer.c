#include <stdio.h>
#include <stdbool.h>

// ��ʱ���ص�����
typedef void (*timer_callback_t)(void);

// ģ�ⶨʱ���ṹ
struct timer {
    unsigned int interval_ms;
    timer_callback_t callback;
    bool is_active;
};

// ��ʼ����ʱ��
void timer_init(struct timer *t, unsigned int interval, timer_callback_t cb) {
    t->interval_ms = interval;
    t->callback = cb;
    t->is_active = false;
}

// ������ʱ��
void timer_start(struct timer *t) {
    t->is_active = true;
    printf("Timer started with interval %d ms\n", t->interval_ms);
}

// ģ�ⶨʱ���жϷ�������
void timer_isr(struct timer *t) {
    if (t->is_active) {
        printf("Timer expired! Calling callback...\n");
        t->callback();
    }
}

// �û�����Ļص�����
void user_timer_callback(void) {
    printf("User callback: Timer event handled\n");
}

int main() {
    struct timer my_timer;
    
    // ��ʼ����ʱ�������ûص�
    timer_init(&my_timer, 1000, user_timer_callback);
    timer_start(&my_timer);
    
    // ģ�ⶨʱ������
    timer_isr(&my_timer);
    
    return 0;
}