/*************************************************************************
	> File Name: system_conf.h
	> Author: 
	> Mail: 
	> Created Time: 2017年12月27日 星期三 10时01分28秒
 ************************************************************************/

#ifndef __SYSTEM_CONF_H
#define __SYSTEM_CONF_H

#include "stdint.h"

/*************************************************************************
                                clock related
 ************************************************************************/
#define FREQ_50MHZ                                  50000000
#define FREQ_100MHZ                                 100000000
#define FREQ_200MHZ                                 200000000
#define FREQ_300MHZ                                 300000000
#define FREQ_400MHZ                                 400000000

#define FCLK                                        FREQ_400MHZ
#define HCLK                                        FREQ_200MHZ
#define PCLK                                        FREQ_100MHZ

#if (FCLK == FREQ_400MHZ)
    //set FCLK frequency as 400MHZ
    #define MDIV                                    ((uint32_t)0x5C)
    #define PDIV                                    ((uint32_t)0x1)
    #define SDIV                                    ((uint32_t)0x1)
#elif (FCLK == FREQ_300MHZ)
    //set FCLK frequency as 304MHZ
    #define MDIV                                    ((uint32_t)0x44)
    #define PDIV                                    ((uint32_t)0x1)
    #define SDIV                                    ((uint32_t)0x1)
#else
    //Set FCLK as 271.5MHZ
    #define MDIV                                    ((uint32_t)0xad)
    #define PDIV                                    ((uint32_t)0x2)
    #define SDIV                                    ((uint32_t)0x2)
#endif

#if (FCLK/2 == HCLK)
    #define HDIVN                                   1
#elif (FCLK/4 == HCLK)
    #define HDIVN                                   2
    #define HCLK4_HALF                              0
#elif (FCLK/8 == HCLK)
    #define HDIVN                                   2
    #define HCLK4_HALF                              1
#elif (FCLK/3 == HCLK)
    #define HDIVN                                   3
    #define HCLK3_HALF                              0
#elif (FCLK/6 == HCLK)
    #define HDIVN                                   3
    #define HCLK3_HALF                              1
#else
    #define HDIVN                                   0
#endif

#if (HCLK/2 == PCLK)
    #define PDIVN                                   1
#else
    #define PDIVN                                   0
#endif

#endif
