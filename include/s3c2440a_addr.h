/*************************************************************************
	> File Name: s3c2440a_addr.h
	> Author: 
	> Mail: 
	> Created Time: 2017年12月23日 星期六 00时05分44秒
 ************************************************************************/

#ifndef __S3C2440A_ADDR_H
#define __S3C2440A_ADDR_H

typedef char            int8_t;
typedef short           int16_t;
typedef int             int32_t;
typedef long            int64_t;

typedef unsigned char   uint8_t;
typedef unsigned short  uint16_t;
typedef unsigned int    uint32_t;
typedef unsigned long   uint64_t;

#define CLOCK_POWER_BASE_ADDR                       ((uint64_t)0x4C000000)

#define WATCHDOG_BASE_ADDR                          ((uint64_t)0x53000000)

#define GPIOA_BASE_ADDR                             ((uint64_t)0x56000000)
#define GPIOB_BASE_ADDR                             ((uint64_t)0x56000010)
#define GPIOC_BASE_ADDR                             ((uint64_t)0x56000020)
#define GPIOD_BASE_ADDR                             ((uint64_t)0x56000030)
#define GPIOE_BASE_ADDR                             ((uint64_t)0x56000040)
#define GPIOF_BASE_ADDR                             ((uint64_t)0x56000050)
#define GPIOG_BASE_ADDR                             ((uint64_t)0x56000060)
#define GPIOH_BASE_ADDR                             ((uint64_t)0x56000070)
#define GPIOJ_BASE_ADDR                             ((uint64_t)0x560000b0)

#endif
