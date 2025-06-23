typedef enum {
    LED_OFF = 0,
    LED_ON,
    LED_BLINK_SLOW,
    LED_BLINK_FAST,
    LED_BREATH
} LedState;

typedef struct {
    GPIO_TypeDef *port;  // LED���ڵ�GPIO�˿�
    uint16_t pin;        // LED��Ӧ������
    LedState default_state; // Ĭ��״̬
    const char *name;    // LED����(��ѡ)
} LedConfig;

const LedConfig led_config_table[] = {
    {GPIOA, GPIO_PIN_0, LED_OFF, "Power LED"},
    {GPIOB, GPIO_PIN_5, LED_BLINK_SLOW, "Status LED"},
    {GPIOC, GPIO_PIN_13, LED_ON, "Error LED"},
    // ��Ӹ���LED����...
};

#define LED_COUNT (sizeof(led_config_table) / sizeof(LedConfig))

typedef struct {
    uint8_t led_id;
    LedState state;
    uint32_t duration; // ״̬����ʱ��(ms)
} LedSequenceStep;

const LedSequenceStep boot_sequence[] = {
    {0, LED_ON, 200},
    {1, LED_ON, 200},
    {2, LED_ON, 200},
    {0, LED_OFF, 200},
    {1, LED_OFF, 200},
    {2, LED_OFF, 200},
    {0, LED_BLINK_FAST, 1000},
    {1, LED_BLINK_SLOW, 0} // 0��ʾ���޳���ʱ��
};s