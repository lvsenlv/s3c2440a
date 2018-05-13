/*************************************************************************
	> File Name: nand_flash.h
	> Author: 
	> Mail: 
	> Created Time: 2018年05月13日 星期日 23时00分00秒
 ************************************************************************/

#ifndef __NAND_FLASH_H
#define __NAND_FLASH_H

#include "s3c2440a.h"

/*
    K9F2G08U0C(NAND FLASH):
        Page size: 2048 byte
        Row address: A0 - A11
        Col address: A12 - A28
    
    2048 = 2 ^ 11
    So 11bit data can indicate the row address, i.e A11 is always 0
    2048-1=2047=0x7FF

    Thereforce,
    address&0x7FF is the row address
    address>>11 is the column address
*/

//TACLS TWRPH0 TWRPH1: refer to s3c2440a datasheet P217 Figure6-3

/*
    TCLS: CLE Setup Time
    TALS: ALE Setup Time
        
    (HCLK * TACLS) > max(TCLS, TALS)
    According to specification P12 Table2.9
    TCLS_min: 12ns
    TALS_min: 12ns

    In start_up.s, HCLK is 100MHZ, so T=1/100MHZ=10ns
    Thereforce:
        (HCLK * TACLS) > max(TCLS, TALS)
        (10   * TACLS) > max(12,   12)
        (10   * 2    ) > 12
    
    TACLS = 2
    According to nand flash specification, it allows to send CLE/ALE at the same of sending WE signal,
    so TACLS can be set as 0
*/
#define NF_TACLS                            0



/*
    TWP: WE Pulse Width
    
    (HCLK * (TWRPH0 + 1 )) > TWP
    According to specification P12 Table2.9
    TWP_min: 12ns
    
    Thereforce:
        (HCLK * (TWRPH0 + 1 )) > TWP_min
        (10   * (TWRPH0 + 1 )) > 12
        (10   * (1      + 1 )) > 12
        
    TWRPH0 = 1
*/
#define NF_TWRPH0                           3



/*
    TCLH: CLE Hold Time
    TALH: ALE Hold Time
    
    (HCLK * (TWRPH1 + 1 )) > max(TCLH, TALH)
    According to specification P12 Table2.9
    TCLH_min: 5ns
    TALH_min: 5ns
    
    Thereforce:
        (HCLK * (TWRPH1 + 1 )) > max(TCLH, TALH)
        (10   * (TWRPH1 + 1 )) > max(5,    5)
        (10   * (0      + 1 )) > 5

    TWRPH1 = 0
*/
#define NF_TWRPH1                           0

#define NF_PAGE_SIZE                        2048
#define NF_PAGE_MASK                        (NF_PAGE_SIZE-1)

#define NF_CMD_RESET                        0xFF
#define NF_CMD_READ_1ST                     0x00
#define NF_CMD_READ_2ND                     0x30
#define NF_CMD_READ_ID                      0x90

#define NF_SELECT() \
        do \
        { \
            NAND->NFCONT &= ~(0x1 << 1); \
            delay(10); \
        }while(0)

#define NF_DESELECT() \
        do \
        { \
            NAND->NFCONT |= 0x1 << 1; \
        }while(0)

#define NF_WAIT_IDLE() \
        do \
        { \
            while(!(NAND->NFSTAT & 0x1)) \
            { \
                delay(10); \
            } \
        }while(0)

#define NF_WRITE_CMD(cmd) \
        do \
        { \
            NAND->NFCMMD = (uint8_t)cmd; \
        }while(0)

#define NF_MAIN_ECC_UNLOCK() \
        do \
        { \
            NAND->NFCONT &= ~(0x1 << 5); \
        }while(0)

#define NF_MAIN_ECC_LOCK() \
        do \
        { \
            NAND->NFCONT |= 0x1 << 5; \
        }while(0)
        
#define NF_SPARE_ECC_UNLOCK() \
        do \
        { \
            NAND->NFCONT &= ~(0x1 << 6); \
        }while(0)
        
#define NF_SPARE_ECC_LOCK() \
        do \
        { \
            NAND->NFCONT |= 0x1 << 6; \
        }while(0)
#endif
