/***************************************************************
文件名	: 	 bsp_beep.h
作者	   : 李宁
版本	   : V1.0
描述	   : 蜂鸣器驱动头文件。
其他	   : 无
***************************************************************/
#ifndef __BSP_BEEP_H
#define __BSP_BEEP_H

#include "imx6ul.h"


/* 函数声明 */
void beep_init(void);
void beep_switch(int status);
#endif

