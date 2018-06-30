/*************************************************************************
	> File Name: main.c
	> Author: 
	> Mail: 
	> Created Time: 2018年05月13日 星期日 23时00分00秒
 ************************************************************************/

#include "led.h"
#include "easy_libc.h"
#include "key.h"
#include "timer.h"

int main(void)
{
    key_init();
    timer_init();

    while(1)
    {
        LED_BUSY_ON();
        sleep(1);
        LED_BUSY_OFF();
        sleep(1);
    }

    return 0;
}
