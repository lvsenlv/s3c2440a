/*************************************************************************
	> File Name: system_init.c
	> Author: 
	> Mail: 
	> Created Time: 2017年12月23日 星期六 00时26分33秒
 ************************************************************************/

#include "s3c2440a_reg.h"

void system_init(void)
{
    WATCHDOG_REG *ptr_reg = WATCHDOG;
    ptr_reg->con &= ~(0x1 << 5);
}
