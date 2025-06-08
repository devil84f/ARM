#include <stdio.h>

#define MAX_EVENTS 10

// �����¼��ṹ��
typedef struct Event {
    int id;                     // �¼�ID
    struct Event* next;         // ����ָ��
} Event;

// �¼��غ���������ͷָ��
static Event event_pool[MAX_EVENTS];  // ��̬�¼���
static Event* free_list = NULL;       // ���нڵ�����
static Event* event_queue = NULL;     // �¼�����

// ��ʼ���¼��أ������нڵ�����������
void init_event_pool() {
    for (int i = 0; i < MAX_EVENTS - 1; i++) {
        event_pool[i].next = &event_pool[i + 1];
    }
    event_pool[MAX_EVENTS - 1].next = NULL;
    free_list = &event_pool[0];
}

// ��ȡһ�����нڵ�
Event* get_free_event() {
    if (free_list == NULL) return NULL; // û�п��нڵ�
    Event* node = free_list;
    free_list = free_list->next;
    node->next = NULL;
    return node;
}

// ����һ���¼��ڵ�
void release_event(Event* node) {
    node->next = free_list;
    free_list = node;
}

// ���¼����������һ���¼�
void enqueue_event(int id) {
    Event* new_event = get_free_event();
    if (new_event == NULL) {
        printf("�¼����������޷�����¼�\n");
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

    printf("�¼� %d �Ѽ����¼�����\n", id);
}

// ģ�⴦���¼��ĺ���
void handle_event(int id) {
    printf("���ڴ����¼� %d\n", id);
}

// �����¼������е������¼�
void process_events() {
    while (event_queue != NULL) {
        Event* current = event_queue;
        event_queue = event_queue->next;

        handle_event(current->id);
        release_event(current); // ���սڵ�
    }
}

int main() {
    init_event_pool();

    // ģ������¼�
    enqueue_event(1);
    enqueue_event(2);
    enqueue_event(3);

    // ���������¼�
    process_events();

    // �ٴμ����¼�
    enqueue_event(4);
    process_events();

    return 0;
}
