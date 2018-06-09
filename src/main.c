/*************************************************************************
	> File Name: main.c
	> Author: 
	> Mail: 
	> Created Time: 2018年05月13日 星期日 23时00分00秒
 ************************************************************************/

#include "led.h"
#include "uart.h"
#include "easy_libc.h"

int main(void)
{
    char buf[1024];
    int i = 0;
    int res;
    
    do
    {
        LED_READY_ON();
        delay(0xFFFFF);
        LED_READY_OFF();
        delay(0xFFFFF);

        res = int_to_strn(buf, sizeof(buf), i++);
        buf[res++] = '\r';
        buf[res++] = '\n';
        buf[res++] = '\0';

        uart_send_str(buf);
    }while(1);

    return 0;
}
