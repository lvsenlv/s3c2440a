/*************************************************************************
	> File Name: s3c2440a.h
	> Author: 
	> Mail: 
	> Created Time: 2018年05月13日 星期日 23时00分00秒
 ************************************************************************/

#ifndef __S3C2440A_H
#define __S3C2440A_H

//Clock is initialized in start_up.s
#define FCLK                                400 //Unit: MHZ
#define HCLK                                100 //Unit: MHZ
#define PCLK                                100 //Unit: MHZ

typedef char                                int8_t;
typedef short                               int16_t;
typedef int                                 int32_t;
typedef unsigned char                       uint8_t;
typedef unsigned short                      uint16_t;
typedef unsigned int                        uint32_t;

#define __IO                                volatile
#define PACKED                              __attribute__((packed))
#define ALIGN_4K                            __attribute__((aligned(4)))
#define PRINTF_FORMAT                       __attribute__((__format__ (__printf__, 1, 2)))
#define WARNING                             "[Warning] "
#define ERROR                               "[Error] "
#define FATAL                               "[Fatal Error] "

#define NOP() \
        do \
        { \
            __asm__( \
                "nop\n" \
            ); \
        }while(0)

#define GPIO_BASE_ADDR                      ((uint32_t)0x56000000)
#define GPIOF_BASE_ADDR                     (GPIO_BASE_ADDR + 0x50)
#define GPIOG_BASE_ADDR                     (GPIO_BASE_ADDR + 0x60)
#define GPIOH_BASE_ADDR                     (GPIO_BASE_ADDR + 0x70)
#define GPIOF                               ((gpio_reg_t *)GPIOF_BASE_ADDR)
#define GPIOG                               ((gpio_reg_t *)GPIOG_BASE_ADDR)
#define GPIOH                               ((gpio_reg_t *)GPIOH_BASE_ADDR)

#define SDRAM_START_ADDR                    ((uint32_t)0x30000000)

#define NAND_BASE_ADDR                      ((uint32_t)0x4E000000)
#define NAND                                ((nand_reg_t *)NAND_BASE_ADDR)

#define UART0_BASE_ADDR                     ((uint32_t)0x50000000)
#define UART0                               ((uart_reg_t *)UART0_BASE_ADDR)

#define EINT_BASE_ADDR                      ((uint32_t)0x56000088)
#define EINT                                ((eint_reg_t *)EINT_BASE_ADDR)
#define INTERRUPT_BASE_ADDR                 ((uint32_t)0x4A000000)
#define INTERRUPT                           ((int_reg_t *)INTERRUPT_BASE_ADDR)
#define EINT0_MASK                          (0x1 << 0)
#define EINT2_MASK                          (0x1 << 2)
#define EINT8_23_MASK                       (0x1 << 5)
#define EINT11_MASK                         (0x1 << 11) //It is only used for EINT->EINTPEND / EINT->EINTMASK
#define INT_TIMER0_MASK                     (0x1 << 10)
#define INT_TIMER1_MASK                     (0x1 << 11)

#define TIMER_BASE_ADDR                     ((uint32_t)0x51000000)
#define TIMER                               ((timer_reg_t *)TIMER_BASE_ADDR)

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
}uart_reg_t;

typedef struct eint_register_struct {
    __IO uint32_t EXTINT0;
    __IO uint32_t EXTINT1;
    __IO uint32_t EXTINT2;
    __IO uint32_t EINTFLT0;
    __IO uint32_t EINTFLT1;
    __IO uint32_t EINTFLT2;
    __IO uint32_t reserved;
    __IO uint32_t EINTMASK;
    __IO uint32_t EINTPEND;
}eint_reg_t;

typedef struct int_register_struct {
    __IO uint32_t SRCPND;
    __IO uint32_t INTMOD;
    __IO uint32_t INTMSK;
    __IO uint32_t PRIORITY;
    __IO uint32_t INTPND;
    __IO uint32_t INTOFFSET;
    __IO uint32_t SUBSRCPND;
    __IO uint32_t INTSUBMSK;
}int_reg_t;

typedef struct timer_register_struct {
    __IO uint32_t TCFG0;
    __IO uint32_t TCFG1;
    __IO uint32_t TCON;
    __IO uint32_t TCNTB0;
    __IO uint32_t TCMPB0;
    __IO uint32_t TCNTO0;
    __IO uint32_t TCNTB1;
    __IO uint32_t TCMPB1;
    __IO uint32_t TCNTO1;
    __IO uint32_t TCNTB2;
    __IO uint32_t TCMPB2;
    __IO uint32_t TCNTO2;
    __IO uint32_t TCNTB3;
    __IO uint32_t TCMPB3;
    __IO uint32_t TCNTO3;
    __IO uint32_t TCNTB4;
    __IO uint32_t TCNTO4;
}timer_reg_t;

#endif
