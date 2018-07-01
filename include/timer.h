/*************************************************************************
	> File Name: timer.h
	> Author: 
	> Mail: 
	> Created Time: 2018年06月30日 星期六 16时40分28秒
 ************************************************************************/

#ifndef __TIMER_H
#define __TIMER_H

#include "s3c2440a.h"

#define sleep(ti)                           timer_delay(ti*1000000)
#define msleep(ti)                          timer_delay(ti*1000)
#define usleep(ti)                          timer_delay(ti)

#define TIMER0_1_PRESCALER                  249  //Prescaler: 249+1
#define TIMER0_1_DIVIDER                    0x22 //Divider: 8, it is only used for TIMER->TCFG1
#define TIMER0_1_ACTUAL_DIVIDER             8

__IO extern uint32_t g_delay_count;

void timer_init(void);
void timer_interrupt_init(void);
void timer_delay(uint32_t time_us);

#endif
