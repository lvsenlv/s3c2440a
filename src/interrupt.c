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
        case INT_TIMER0_MASK:
            LED_READY_TOGGLE();
            break;
        case INT_TIMER1_MASK:
            if(0 != g_delay_count)
            {
                g_delay_count--;
            }
            
            break;
        case EINT0_MASK:
            printf("Detect external interrupt 0\n");
            break;
        case EINT2_MASK:
            printf("Detect external interrupt 2\n");
            break;
        case EINT8_23_MASK:
            eint_offset = EINT->EINTPEND;
            if(eint_offset & EINT11_MASK) //EINT11
            {
                EINT->EINTPEND |= EINT11_MASK;
                printf("Detect external interrupt 11\n");
            }
            
            break;
        default :
            printf(WARNING"Detect unknown interrupt signal, offset=0x%x\n", offset);
            break;
    }

    //Clear interrupt
    INTERRUPT->SRCPND |= 1 << offset;
    INTERRUPT->INTPND |= 1 << offset;
}
