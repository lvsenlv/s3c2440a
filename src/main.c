/*************************************************************************
	> File Name: main.c
	> Author: 
	> Mail: 
	> Created Time: 2018年05月13日 星期日 23时00分00秒
 ************************************************************************/

#include "led.h"
#include "uart.h"
#include "easy_libc.h"

static inline void delay(uint32_t count)
{
    for(; count > 0; count--);
}

int main(void)
{
    int i = 0;
    
    do
    {
        LED_READY_ON();
        delay(0x5FFFF);
        LED_READY_OFF();
        delay(0x5FFFF);

        printf("%d\r\n", i++);
    }while(1);

    return 0;
}
