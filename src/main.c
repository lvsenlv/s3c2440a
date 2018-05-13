/*************************************************************************
	> File Name: main.c
	> Author: 
	> Mail: 
	> Created Time: 2018年05月13日 星期日 23时00分00秒
 ************************************************************************/

#include "led.h"

int main(void)
{
    do
    {
        LED_BUSY_ON();
        delay(0xFFFF);
        LED_BUSY_OFF();
        LED_ERR_ON();
        delay(0xFFFF);
        LED_ERR_OFF();
        LED_READY_ON();
        delay(0xFFFF);
        LED_READY_OFF();
    }while(1);

    return 0;
}
