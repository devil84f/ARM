/**************************************************************
文件名	: 	 mian.c
作者	   : 李宁
版本	   : V1.0
描述	   : I.MX6U开发板裸机实验7 按键输入实验
其他	   : 本实验主要学习使用状态机串联LED/BEEP/KEY。
**************************************************************/
#include "bsp_clk.h"
#include "bsp_delay.h"
#include "bsp_led.h"
#include "bsp_beep.h"
#include "bsp_key.h"

/*
 * @description	: main函数
 * @param 		: 无
 * @return 		: 无
 */
int main(void)
{
	int keyvalue = 0;
	
	clk_enable();		/* 使能所有时钟	 */
	led_init();			/* 初始化led	*/
	beep_init();		/* 初始化beep	*/
	key_init();			/* 初始化key 	*/

	/* 状态机枚举类型 */
	typedef enum __STATE_VALUE {
		STATE_0 	, /* 状态0：空 			*/
		STATE_1 	, /* 状态1：LED亮		*/
		STATE_2 	, /* 状态2：LED亮BEEP响 */
		STATE_3 	, /* 状态3：BEEP响      */
	} state_value;

	state_value STATE = STATE_0;
	while(1)			
	{	
		keyvalue = key_getvalue();
		if(keyvalue)
		{
			STATE = (STATE + 1) % 4;
			switch (STATE) {
                case STATE_0:
                    led_switch(LED0, OFF);  // 确保LED灭
                    beep_switch(OFF);       // 确保蜂鸣器关
                    break;
                case STATE_1:
                    led_switch(LED0, ON);
                    beep_switch(OFF);
                    break;
                case STATE_2:
                    led_switch(LED0, ON);
                    beep_switch(ON);
                    break;
                case STATE_3:
                    led_switch(LED0, OFF);
                    beep_switch(ON);
                    break;
            }
		}
		delay(10);
	}

	return 0;
}
