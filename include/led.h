/*************************************************************************
	> File Name: led.h
	> Author: 
	> Mail: 
	> Created Time: 2017年12月23日 星期六 18时49分40秒
 ************************************************************************/

#ifndef __LED_H
#define __LED_H

#include "s3c2440a_reg.h"

void led_init(void);

#define LED1_ON                                      (GPIOF->dat &= ~(0x1<<4))
#define LED2_ON                                      (GPIOF->dat &= ~(0x1<<5))
#define LED4_ON                                      (GPIOF->dat &= ~(0x1<<6))

#define LED1_OFF                                     (GPIOF->dat |= 0x1<<4)
#define LED2_OFF                                     (GPIOF->dat |= 0x1<<5)
#define LED4_OFF                                     (GPIOF->dat |= 0x1<<6)



#endif
