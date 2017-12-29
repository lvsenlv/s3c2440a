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

    char buf[20];
    int i = 0;

    while(1)
    {
        LED1_ON;
        uart_print_int_num(i++);
        delay(0x2FFFFF);
        LED1_OFF;
        delay(0x2FFFFF);
    }
    
    return 0;
}
