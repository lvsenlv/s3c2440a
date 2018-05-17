/*************************************************************************
	> File Name: s3c2440a.h
	> Author: 
	> Mail: 
	> Created Time: 2018年05月13日 星期日 23时00分00秒
 ************************************************************************/

#ifndef __S3C2440A_H
#define __S3C2440A_H

typedef char            int8_t;
typedef short           int16_t;
typedef int             int32_t;
typedef unsigned char   uint8_t;
typedef unsigned short  uint16_t;
typedef unsigned int    uint32_t;

#define __IO                                volatile
#define PACKED                              __attribute__((packed))
#define ALIGN_4K                            __attribute__((aligned(4)))
#define PRINTF_FORMAT                       __attribute__((__format__ (__printf__, 1, 2)))


#define NOP() \
        do \
        { \
            __asm__( \
                "nop\n" \
            ); \
        }while(0)

//Clock is initialized in start_up.s
#define FCLK                                400 //Unit: MHZ
#define HCLK                                100 //Unit: MHZ
#define PCLK                                100 //Unit: MHZ

#define GPIO_BASE_ADDR                      ((uint32_t)0x56000000)
#define GPIOF_BASE_ADDR                     (GPIO_BASE_ADDR + 0x50)
#define GPIOF                               ((gpio_reg_t *)GPIOF_BASE_ADDR)

#define SDRAM_START_ADDR                    ((uint32_t)0x30000000)

/*
    Modify NAND_BASE: must modify following options at the same time
        nand_flash.c: sys_init() --- //Config NAND->NFCONT
*/
#define NAND_BASE                           ((uint32_t)0x4E000000)
#define NAND                                ((nand_reg_t *)NAND_BASE)

#define UART0_BASE                          ((uint32_t)0x50000000)
#define UART0                               ((uart_register_t *)UART0_BASE)

typedef struct gpio_register_struct {
    __IO uint32_t CON;
    __IO uint32_t DAT;
   __IO uint32_t UP;
}gpio_reg_t;

typedef struct nand_register_struct {
    __IO uint32_t NFCONF;
    __IO uint32_t NFCONT;
    __IO uint32_t NFCMMD;
    __IO uint32_t NFADDR;
    __IO uint32_t NFDATA;
    __IO uint32_t NFMECCD0;
    __IO uint32_t NFMECCD1;
    __IO uint32_t NFSECCD;
    __IO uint32_t NFSTAT;
    __IO uint32_t NFESTAT0;
    __IO uint32_t NFESTAT1;
    __IO uint32_t NFMECC0;
    __IO uint32_t NFMECC1;
    __IO uint32_t NFSECC;
    __IO uint32_t NFSBLK;
    __IO uint32_t NFEBLK;
}nand_reg_t;

typedef struct uart_register_struct {
    __IO uint32_t ULCON;
    __IO uint32_t UCON;
    __IO uint32_t UFCON;
    __IO uint32_t UMCON;
    __IO uint32_t UTRSTAT;
    __IO uint32_t UERSTAT;
    __IO uint32_t UFSTAT;
    __IO uint32_t UMSTAT;
    __IO uint32_t UTXH;
    __IO uint32_t URXH;
    __IO uint32_t UBRDIV;
}uart_register_t;

static inline void delay(uint32_t count)
{
    for(; count > 0; count--);
}

#endif
