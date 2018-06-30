/*************************************************************************
	> File Name: led.h
	> Author: 
	> Mail: 
	> Created Time: 2018年05月13日 星期日 23时00分00秒
 ************************************************************************/

#ifndef __LED_H
#define __LED_H

#include "s3c2440a.h"

#define LED_BUSY_GPIO                       GPIOF
#define LED_BUSY_PIN                        4

#define LED_ERR_GPIO                        GPIOF
#define LED_ERR_PIN                         5

#define LED_READY_GPIO                      GPIOF
#define LED_READY_PIN                       6

#define LED_BUSY_ON() \
        do \
        { \
            LED_BUSY_GPIO->DAT &= ~(0x1 << LED_BUSY_PIN); \
        }while(0)
        
#define LED_BUSY_OFF() \
        do \
        { \
            LED_BUSY_GPIO->DAT |= 0x1 << LED_BUSY_PIN; \
        }while(0)

#define LED_ERR_ON() \
        do \
        { \
            LED_ERR_GPIO->DAT &= ~(0x1 << LED_ERR_PIN); \
        }while(0)
        
#define LED_ERR_OFF() \
        do \
        { \
            LED_ERR_GPIO->DAT |= 0x1 << LED_ERR_PIN; \
        }while(0)
        
#define LED_READY_ON() \
        do \
        { \
            LED_READY_GPIO->DAT &= ~(0x1 << LED_READY_PIN); \
        }while(0)
        
#define LED_READY_OFF() \
        do \
        { \
            LED_READY_GPIO->DAT |= 0x1 << LED_READY_PIN; \
        }while(0)

#define LED_READY_TOGGLE() \
        do \
        { \
            LED_READY_GPIO->DAT ^= 0x1 << LED_READY_PIN; \
        }while(0)
#endif
