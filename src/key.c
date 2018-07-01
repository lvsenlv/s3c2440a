/*************************************************************************
	> File Name: key.c
	> Author: 
	> Mail: 
	> Created Time: 2018年06月30日 星期六 16时40分04秒
 ************************************************************************/

#include "key.h"

void key_init(void)
{
    //Set GPIOF0 GPIOF2 as EINT
    GPIOF->CON &= ~(0x3 | 0x30);
    GPIOF->CON |= 0x2 | 0x20;

    //Set GPIOG3 as EINT
    GPIOG->CON &= ~(0x3 << (2*3));
    GPIOG->CON |= 0x2 << (2*3);
}

void key_interrupt_init(void)
{
    //Enable EINT11 for GPIOG3
    EINT->EINTMASK &= ~EINT_MASK_EINT11;
    
    /*
     * ARB_SEL0 = 00b, ARB_MODE0 = 0: REQ1 > REQ3, i.e. EINT0 > EINT2
     * Arbitrator 1~6 need not to set
     * EINT0 > EINT2 > EINT11
     */
    INTERRUPT->PRIORITY = INTERRUPT->PRIORITY & ((~0x01) | (0x3<<7));
    
    //Clear interrupt
    //INTERRUPT->SRCPND |= EINT0_MASK | EINT2_MASK | EINT8_23_MASK;
    
    //Enable EINT0 EINT2 EINT11
    INTERRUPT->INTMSK &= ~(INT_MASK_EINT0 | INT_MASK_EINT2 | INT_MASK_EINT8_23);
}
