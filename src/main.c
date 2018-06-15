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
    char buf[64];

    int_to_strn(buf, sizeof(buf), 123);
    easy_printf("test\n");
    
    do
    {
        LED_READY_ON();
        delay(0xFFFFF);
        LED_READY_OFF();
        delay(0xFFFFF);
    }while(1);

    return 0;
}
