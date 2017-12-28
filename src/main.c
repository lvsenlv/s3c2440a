/*************************************************************************
	> File Name: main.c
	> Author: 
	> Mail: 
	> Created Time: 2017年12月23日 星期六 00时04分46秒
 ************************************************************************/

#include "led.h"
#include "common.h"
#include "uart.h"

int main(void)
{
    led_init();
    uart_init();

    uint8_t ch = 'a';
    
    while(1)
    {
        LED1_ON;
        delay(777777);
        LED1_OFF;
        LED2_ON;
        delay(777777);
        LED2_OFF;
        LED4_ON; 
        delay(777777);
        LED4_OFF;
        
        UART0->utxh = ch;
        while(!(UART0->utrstat & 0x2))
        {}
        
        ch++;
        if('z' < ch)
            ch = 'a';
    }
    
    return 0;
}
