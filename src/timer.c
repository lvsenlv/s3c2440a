/*************************************************************************
	> File Name: timer.c
	> Author: 
	> Mail: 
	> Created Time: 2018年06月30日 星期六 16时40分12秒
 ************************************************************************/

#include "timer.h"

uint32_t g_delay_count = 0;

void timer_init(void)
{
    TIMER->TCFG0 &= ~0xFF;
    TIMER->TCFG0 |= TIMER0_1_PRESCALER;
    TIMER->TCFG1 &= ~0xFF;
    TIMER->TCFG1 |= TIMER0_1_DIVIDER;

    TIMER->TCNTB0 = (PCLK * 1000000 / (TIMER0_1_PRESCALER+1) / TIMER0_1_DIVIDER) & 0xFFFF;
    TIMER->TCMPB0 = 0;

    //Configure timer0
    TIMER->TCON |= 0x2; //Set TCNTB0 and TCMPB0 into internal TCNT0 and TCMP0
    TIMER->TCON &= ~0xF;
    TIMER->TCON |= 0x9; //Set as auto-reload mode and start timer0
}

void timer_interrupt_init(void)
{
    //Clear interrupt
    INTERRUPT->SRCPND |= INT_TIMER0_MASK | INT_TIMER1_MASK;
    
    //Enable INT_TIMER0 INT_TIMER1
    INTERRUPT->INTMSK &= ~(INT_TIMER0_MASK | INT_TIMER1_MASK);
}

/*
 *  Briefs: Achieve based on TIMER1
*/
void timer_delay(uint32_t time_us)
{
    uint32_t frequency;

    /*
        If PCLK = 100MHZ, so frequency = 50000, and it means periods is 20us
    */
    frequency = (PCLK * 1000000 / (TIMER0_1_PRESCALER+1) / TIMER0_1_DIVIDER) & 0xFFFF;
    TIMER->TCMPB1 = 0;
    
    if(1000 > time_us) //Precision: us
    {
        TIMER->TCNTB1 = 1; //Occurs interrupt per 1/frequency second
        g_delay_count = time_us * frequency / 1000000;
    }
    else if(1000000 > time_us) //Precision: ms
    {
        TIMER->TCNTB1 = frequency / 1000; //Occurs interrupt per 1ms
        g_delay_count = time_us / 1000;
    }
    else
    {
        TIMER->TCNTB1 = frequency/10; //Occurs interrupt per 100ms
        g_delay_count = time_us / 100000;
    }

    if(0 == g_delay_count)
    {
        TIMER->TCON &= 0x1 << 8; //Stop TIMER1
        return;
    }

    TIMER->TCON |= 0x2 << 8; //Set TCNTB1 and TCMPB1 into internal TCNT1 and TCMP1
    TIMER->TCON &= ~(0xF << 8);
    TIMER->TCON |= 0x9 << 8; //Set as auto-reload mode and start timer1

    while(0 != g_delay_count);

    TIMER->TCON &= 0x1 << 8; //Stop TIMER1
}
