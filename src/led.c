/*************************************************************************
	> File Name: led.c
	> Author: 
	> Mail: 
	> Created Time: 2017年12月23日 星期六 18时49分25秒
 ************************************************************************/

#include "led.h"

 void led_init(void)
 {
     GPIO_REG *ptr_reg = GPIOF;
     ptr_reg->con &= ~((0x3<<8) | (0x3<<10) | (0x1<<12));
     ptr_reg->con |= (0x1<<8) | (0x1<<10) | (0x1<<12);
     
     LED1_OFF;
     LED2_OFF;
     LED4_OFF;

 }


