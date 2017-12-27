/*************************************************************************
	> File Name: system_init.c
	> Author: 
	> Mail: 
	> Created Time: 2017年12月23日 星期六 00时26分33秒
 ************************************************************************/

#include "s3c2440a_reg.h"
#include "system_conf.h"

void system_init(void)
{
    WATCHDOG->con &= ~(0x1 << 5);

    //Set up clock >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#if (HDIVN != 0) //Set as asynchronous bus modes
    __asm__(
        "mrc    p15, 0, r1, c1, c0, 0\n"
        "orr    r1, r1, #0xc0000000\n"
        "mcr    p15, 0, r1, c1, c0, 0\n"
    );
#endif

    CLOCK->clkdivn &= ~0x7;
    CLOCK->clkdivn |= (HDIVN << 1) | PDIVN;

    CLOCK->mpllcon &= ~((0xFF << 12) | (0x3F << 4) | 0x3);
    CLOCK->mpllcon |= (MDIV << 12) | (PDIV << 4) | SDIV;

#if ((FCLK/4 == HCLK) || (FCLK/8 == HCLK))
    CLOCK->camdivn &= ~(0x1 << 9);
    CLOCK->camdivn |= HCLK4_HALF << 9;
#elif ((FCLK/3 == HCLK) || (FCLK/6 == HCLK))
    CLOCK->camdivn &= ~(0x1 << 8);
    CLOCK->camdivn |= HCLK3_HALF << 8;
#endif
    //Set up clock <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    
}
