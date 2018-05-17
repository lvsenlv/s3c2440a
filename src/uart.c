/*************************************************************************
	> File Name: uart.c
	> Author: 
	> Mail: 
	> Created Time: 2018年05月13日 星期日 23时00分00秒
 ************************************************************************/

#include "uart.h"

int uart_init(void)
{
    UART0->ULCON &= ~0x7F;
    UART0->ULCON |= (UART_PARITY<<3) | (UART_STOP_BIT<<2) | UART_WORD_LEN;

    UART0->UCON &= ~((0x3<<10) | (0x3<<2)); //UART CLOCK = PCLK
    UART0->UCON |= 0x1 << 2;                //Transmit Mode: Interrupt request or polling mode

    float tmp;
    int div;
    
    tmp = (float)(UART_CLOCK)/(UART_BUADRATE*16);
    div = (int)(tmp + 0.5f) - 1;

    UART0->UBRDIV = div;
    
    return 0;
}

void uart_send_str(const char *str)
{
    __IO uint8_t *transmit_reg = (__IO uint8_t *)&UART0->UTXH;
    __IO uint8_t *transmit_status_reg = (__IO uint8_t *)&UART0->UTRSTAT;
    
    while('\0' != *str)
    {
        *transmit_reg = *str++;
        while(!(*transmit_status_reg & 0x2));
    }
}
