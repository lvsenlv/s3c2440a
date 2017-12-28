/*************************************************************************
	> File Name: uart.c
	> Author: 
	> Mail: 
	> Created Time: 2017年12月28日 星期四 20时38分31秒
 ************************************************************************/

#include "uart.h"
#include "s3c2440a_reg.h"
#include <stdio.h>

void uart_init(void)
{
    GPIOH->con |= 0x2 << 4;
    GPIOH->up = 0x1 << 4;
    
    UART_REG *reg = UART0;
    reg->ulcon &= ~0x3F;
    reg->ulcon |= 0x3;
    reg->ucon &= ~(0x3 << 10);
    reg->ucon |= (0x2 << 10) | (0x1 << 2);
    reg->ubrdiv = UBRDIV;
}

int fputc(int ch, FILE *fp)
{
    UART0->utxh = ch & 0xFF;
    while(!(UART0->utrstat & 0x2))
    {}
    
    return ch;
}
