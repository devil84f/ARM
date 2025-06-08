#include <stdio.h>

#define MAX_EVENTS 10

// 定义事件结构体
typedef struct Event {
    int id;                     // 事件ID
    struct Event* next;         // 链表指针
} Event;

// 事件池和两个链表头指针
static Event event_pool[MAX_EVENTS];  // 静态事件池
static Event* free_list = NULL;       // 空闲节点链表
static Event* event_queue = NULL;     // 事件队列

// 初始化事件池，将所有节点放入空闲链表
void init_event_pool() {
    for (int i = 0; i < MAX_EVENTS - 1; i++) {
        event_pool[i].next = &event_pool[i + 1];
    }
    event_pool[MAX_EVENTS - 1].next = NULL;
    free_list = &event_pool[0];
}

// 获取一个空闲节点
Event* get_free_event() {
    if (free_list == NULL) return NULL; // 没有空闲节点
    Event* node = free_list;
    free_list = free_list->next;
    node->next = NULL;
    return node;
}

// 回收一个事件节点
void release_event(Event* node) {
    node->next = free_list;
    free_list = node;
}

// 向事件队列中添加一个事件
void enqueue_event(int id) {
    Event* new_event = get_free_event();
    if (new_event == NULL) {
        printf("事件池已满，无法添加事件\n");
        return;
    }

    new_event->id = id;
    new_event->next = NULL;

    if (event_queue == NULL) {
        event_queue = new_event;
    } else {
        Event* tail = event_queue;
        while (tail->next != NULL) {
            tail = tail->next;
        }
        tail->next = new_event;
    }

    printf("事件 %d 已加入事件队列\n", id);
}

// 模拟处理事件的函数
void handle_event(int id) {
    printf("正在处理事件 %d\n", id);
}

// 处理事件队列中的所有事件
void process_events() {
    while (event_queue != NULL) {
        Event* current = event_queue;
        event_queue = event_queue->next;

        handle_event(current->id);
        release_event(current); // 回收节点
    }
}

int main() {
    init_event_pool();

    // 模拟加入事件
    enqueue_event(1);
    enqueue_event(2);
    enqueue_event(3);

    // 处理所有事件
    process_events();

    // 再次加入事件
    enqueue_event(4);
    process_events();

    return 0;
}
