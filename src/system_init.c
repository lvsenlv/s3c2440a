/*************************************************************************
	> File Name: system_init.c
	> Author: 
	> Mail: 
	> Created Time: 2017年12月23日 星期六 00时26分33秒
 ************************************************************************/

#include "s3c2440a_reg.h"

void system_init(void)
{
    WATCHDOG->con &= ~(0x1 << 5);

    __asm__(
        "mrc    p15, 0, r1, c1, c0, 0\n"
        "orr    r1, r1, #0xc0000000\n"
        "mcr    p15, 0, r1, c1, c0, 0\n"
    );

    CLOCK->clkdivn = 0;
    CLOCK->clkdivn |= (0x1 << 1) | 0x1;

    CLOCK->mpllcon &= ~((0xFF << 12) | (0x3F << 4) | 0x3);
    CLOCK->mpllcon |= (0x5C << 12) | (0x1 << 4) | 0x1;
    
}
