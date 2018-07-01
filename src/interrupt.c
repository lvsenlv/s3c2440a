/*************************************************************************
	> File Name: interrupt.c
	> Author: 
	> Mail: 
	> Created Time: 2018年06月21日 星期四 19时03分24秒
 ************************************************************************/

#include "s3c2440a.h"
#include "easy_libc.h"
#include "key.h"
#include "timer.h"
#include "led.h"

void IRQ_init(void)
{
    key_interrupt_init();
    timer_interrupt_init();
}

void IRQ_process(void)
{
    uint32_t offset;
    uint32_t eint_offset;

    offset = INTERRUPT->INTOFFSET;

    switch(offset)
    {
        case INT_OFFSET_TIMER0:
            LED_READY_TOGGLE();
            break;
        case INT_OFFSET_TIMER1:
            if(0 != g_delay_count)
            {
                g_delay_count--;
            }
            
            break;
        case INT_OFFSET_EINT0:
            break;
        case INT_OFFSET_EINT2:
            break;
        case INT_OFFSET_EINT8_23:
            eint_offset = EINT->EINTPEND;
            if(eint_offset & EINT_MASK_EINT11) //EINT11
            {
                EINT->EINTPEND |= EINT_MASK_EINT11;
            }
            
            break;
        default :
            printf(WARNING"Detect unknown interrupt signal, offset=0x%x\r\n", offset);
            break;
    }

    //Clear interrupt
    //printf("SRCPND=0x%x, INTPND=0x%x\r\n", INTERRUPT->SRCPND, INTERRUPT->INTPND);
    INTERRUPT->SRCPND = 1 << offset;
    INTERRUPT->INTPND = INTERRUPT->INTPND;
}
