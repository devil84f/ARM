#ifndef _BSP_GPIO_H
#define _BSP_GPIO_H
#define _BSP_KEY_H
#include "imx6ul.h"


/* 
 * 枚举类型和结构体定义 
 */
typedef enum _gpio_pin_direction
{
    kGPIO_DigitalInput = 0U,  		/* 输入 */
    kGPIO_DigitalOutput = 1U, 		/* 输出 */
} gpio_pin_direction_t;

/*
 * GPIO中断触发类型枚举
 */
typedef enum _gpio_interrupt_mode
{
    kGPIO_NoIntmode = 0U, 				/* 无中断功能 */
    kGPIO_IntLowLevel = 1U, 			/* 低电平触发	*/
    kGPIO_IntHighLevel = 2U, 			/* 高电平触发 */
    kGPIO_IntRisingEdge = 3U, 			/* 上升沿触发	*/
    kGPIO_IntFallingEdge = 4U, 			/* 下降沿触发 */
    kGPIO_IntRisingOrFallingEdge = 5U, 	/* 上升沿和下降沿都触发 */
} gpio_interrupt_mode_t;	

/*
 * GPIO配置结构体
 */	
typedef struct _gpio_pin_config
{
    gpio_pin_direction_t direction; 		/* GPIO方向:输入还是输出 */
    uint8_t outputLogic;            		/* 如果是输出的话，默认输出电平 */
	gpio_interrupt_mode_t interruptMode;	/* 中断方式 */
} gpio_pin_config_t;


/* 函数声明 */
void gpio_init(GPIO_Type *base, int pin, gpio_pin_config_t *config);
int gpio_pinread(GPIO_Type *base, int pin);
void gpio_pinwrite(GPIO_Type *base, int pin, int value);
void gpio_intconfig(GPIO_Type* base, unsigned int pin, gpio_interrupt_mode_t pinInterruptMode);
void gpio_enableint(GPIO_Type* base, unsigned int pin);
void gpio_disableint(GPIO_Type* base, unsigned int pin);
void gpio_clearintflags(GPIO_Type* base, unsigned int pin);

#endif
