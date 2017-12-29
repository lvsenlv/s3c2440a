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

void uart_print_char(char ch)
{
    UART0->utxh = ch;
    while(!(UART0->utrstat & 0x2));
}

void uart_print_string(char *str)
{
    while('\0' != *str)
    {
        UART0->utxh = *str++;
        while(!(UART0->utrstat & 0x2));
    }
}

void uart_print_int_num(int32_t num)
{
    uart_print_long_num((int64l_t)num);
}

void uart_print_long_num(int64_t num)
{
    if(0 > num)
    {
        num *= -1;
        UART0->utxh = '-';
        while(!(UART0->utrstat & 0x2));
    }
    
    char buf[32];
    char tmp;
    int i;
    int count = 0;

    buf[0] = '0';
    
    for(i = 0; num != 0; i++)
    {
        buf[i] = (num % 10) + '0';
        num = num / 10;   
    }

    if(0 != i)
        i--;
    
    for(; i >= 0; i--)
    {
        UART0->utxh = buf[i];
        while(!(UART0->utrstat & 0x2));
    }

    UART0->utxh = '\r';
    while(!(UART0->utrstat & 0x2));

    UART0->utxh = '\n';
    while(!(UART0->utrstat & 0x2));
}

