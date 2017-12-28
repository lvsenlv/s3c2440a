/*************************************************************************
	> File Name: s3c2440a_reg.h
	> Author: 
	> Mail: 
	> Created Time: 2017年12月23日 星期六 00时06分03秒
 ************************************************************************/

#ifndef __S3C2440A_REG_H
#define __S3C2440A_REG_H

#include "s3c2440a_addr.h"

typedef struct GPIO_register_struct{
     volatile uint32_t con;
     volatile uint32_t dat;
     volatile uint32_t up;
}GPIO_REG;

typedef struct watchdog_register_struct{
    volatile uint32_t con;
    volatile uint32_t dat;
    volatile uint32_t count;
}WATCHDOG_REG;

typedef struct clock_power_register_struct{
    volatile uint32_t locktime;
    volatile uint32_t mpllcon;
    volatile uint32_t upllcon;
    volatile uint32_t clkcon;
    volatile uint32_t clkslow;
    volatile uint32_t clkdivn;
    volatile uint32_t camdivn;
}CLOCK_REG;

typedef struct uart_register_struct{
    volatile uint32_t ulcon;
    volatile uint32_t ucon;
    volatile uint32_t ufcon;
    volatile uint32_t umcon;
    volatile uint32_t utrstat;
    volatile uint32_t uerstat;
    volatile uint32_t ufstat;
    volatile uint32_t umstat;  
    volatile uint32_t utxh;
    volatile uint32_t urxh;
    volatile uint32_t ubrdiv;
}UART_REG;

#define GPIOA                                       ((GPIO_REG *)GPIOA_BASE_ADDR)
#define GPIOB                                       ((GPIO_REG *)GPIOB_BASE_ADDR)
#define GPIOC                                       ((GPIO_REG *)GPIOC_BASE_ADDR)
#define GPIOD                                       ((GPIO_REG *)GPIOD_BASE_ADDR)
#define GPIOE                                       ((GPIO_REG *)GPIOE_BASE_ADDR)
#define GPIOF                                       ((GPIO_REG *)GPIOF_BASE_ADDR)
#define GPIOG                                       ((GPIO_REG *)GPIOG_BASE_ADDR)
#define GPIOH                                       ((GPIO_REG *)GPIOH_BASE_ADDR)
#define GPIOJ                                       ((GPIO_REG *)GPIOJ_BASE_ADDR)

#define WATCHDOG                                    ((WATCHDOG_REG *)WATCHDOG_BASE_ADDR)

#define CLOCK                                       ((CLOCK_REG *)CLOCK_POWER_BASE_ADDR)

#define UART0                                       ((UART_REG *)UART0_BASE_ADDR)

#endif
