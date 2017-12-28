/*************************************************************************
	> File Name: uart.h
	> Author: 
	> Mail: 
	> Created Time: 2017年12月28日 星期四 20时38分43秒
 ************************************************************************/

#ifndef __UART_H
#define __UART_H

#include "system_conf.h"

#define UART_CLK                                    PCLK
#define BUAD_RATE                                   115200
#define UBRDIV                                      (((int32_t)(UART_CLK/(BUAD_RATE * 16) + 0.5)) - 1)

void uart_init(void);

#endif
