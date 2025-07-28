/***************************************************************
文件名	: 	 bsp_led.h
作者	   : 李宁
版本	   : V1.0
描述	   : LED驱动头文件。
其他	   : 无
***************************************************************/
#ifndef __BSP_LED_H
#define __BSP_LED_H
#include "imx6ul.h"

#define LED0	0

/* 函数声明 */
void led_init(void);
void led_switch(int led, int status);
#endif

