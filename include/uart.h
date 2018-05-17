/*************************************************************************
	> File Name: uart.h
	> Author: 
	> Mail: 
	> Created Time: 2018年05月13日 星期日 23时00分00秒
 ************************************************************************/

#ifndef __UART_H
#define __UART_H

#include "s3c2440a.h"

#ifdef __REDIRECTION
#include "printf.h"
#endif

#define UART_CLOCK                          (PCLK*1000000) //100MHZ
#define UART_BUADRATE                       115200
#define UART_PARITY                         UART_PARITY_NO
#define UART_STOP_BIT                       UART_STOP_BIT_1
#define UART_WORD_LEN                       UART_WORD_LEN_8



#define UART_PARITY_NO                      0x0
#define UART_PARITY_ODD                     0x4
#define UART_PARITY_EVEN                    0x5

#define UART_STOP_BIT_1                     0x0
#define UART_STOP_BIT_2                     0x1

#define UART_WORD_LEN_5                     0x0
#define UART_WORD_LEN_6                     0x1
#define UART_WORD_LEN_7                     0x2
#define UART_WORD_LEN_8                     0x3

#endif
