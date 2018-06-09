/*************************************************************************
	> File Name: uart.c
	> Author: 
	> Mail: 
	> Created Time: 2018年05月13日 星期日 23时00分00秒
 ************************************************************************/

#include "uart.h"

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
