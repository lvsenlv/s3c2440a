/*************************************************************************
	> File Name: sys_init.c
	> Author: 
	> Mail: 
	> Created Time: 2018年05月13日 星期日 23时00分00秒
 ************************************************************************/

#include "led.h"
#include "nand_flash.h"
#include "uart.h"

//#define __SYS_NAND_ECC
#define CODE_SIZE                           (256*NF_PAGE_SIZE) //512Kib
#define CODE_START_ADDR                     4096
/*************************************************************************
                     Instruction about CODE_START_ADDR
At least but not last, while using the tools of openjtag, it needs to input
the target block number. But it takes no effect whatever you input. The 
real load address rely on the value in "AT(xxxx)"
 ************************************************************************/

static inline void delay(uint32_t count)
{
    for(; count > 0; count--);
}

#ifndef __SYS_NAND_ECC
static int sys_nand_read(char *target_ptr, uint32_t source_addr, int size)
{
    if((source_addr & NF_PAGE_MASK) || (size & NF_PAGE_MASK))
        return -1;

    uint32_t i;
    uint32_t j;

    size /= NF_PAGE_SIZE;
    NF_SELECT();

    for(i = 0; i < size; i++)
    {
        NF_WRITE_CMD(NF_CMD_READ_1ST);
        delay(10);
        
        NAND->NFADDR = 0;                               //A0 ~ A7
        NAND->NFADDR = 0;                               //A8 ~ A11
        NAND->NFADDR = (source_addr >> 11) & 0xFF;      //A12 ~ A19
        NAND->NFADDR = (source_addr >> 19) & 0xFF;      //A20 ~ A27
        NAND->NFADDR = (source_addr >> 27) & 0x01;      //A28
        delay(10);
        
        NF_WRITE_CMD(NF_CMD_READ_2ND);
        NF_WAIT_IDLE();

        for(j = 0; j < NF_PAGE_SIZE; j++)
        {
            *target_ptr++ = *(__IO uint8_t *)&NAND->NFDATA;
        }

        source_addr += NF_PAGE_SIZE;
    }
    
    NF_DESELECT();

    return 0;
}
#else
static int sys_nand_read(char *target_ptr, uint32_t source_addr, int size)
{
    if((source_addr & NF_PAGE_MASK) || (size & NF_PAGE_MASK))
        return -1;

    uint32_t i;
    uint32_t j;
    uint32_t main_ecc = 0;
    uint32_t spare_ecc = 0;

    size /= NF_PAGE_SIZE;
    NF_RESET_ECC();
    NF_SELECT();

    for(i = 0; i < size; i++)
    {
        NF_MAIN_ECC_UNLOCK();
        
        NF_WRITE_CMD(NF_CMD_READ_1ST);
        delay(10);
        
        NAND->NFADDR = 0;                               //A0 ~ A7
        NAND->NFADDR = 0;                               //A8 ~ A11
        NAND->NFADDR = (source_addr >> 11) & 0xFF;      //A12 ~ A19
        NAND->NFADDR = (source_addr >> 19) & 0xFF;      //A20 ~ A27
        NAND->NFADDR = (source_addr >> 27) & 0x01;      //A28
        delay(10);
        
        NF_WRITE_CMD(NF_CMD_READ_2ND);
        NF_WAIT_IDLE();

        for(j = 0; j < NF_PAGE_SIZE; j++)
        {
            *target_ptr++ = *(__IO uint8_t *)&NAND->NFDATA;
        }

        source_addr += NF_PAGE_SIZE;
        
        NF_MAIN_ECC_LOCK();
        NF_SPARE_ECC_UNLOCK();

        main_ecc = NF_READ_WORD();
        NAND->NFMECCD0 = (main_ecc & 0xFF) | ((main_ecc & 0xFF00) << 8);
        NAND->NFMECCD1 = ((main_ecc & 0xFF0000) >> 16) | ((main_ecc & 0xFF000000) >> 8);
        
        NF_SPARE_ECC_LOCK();
        
        spare_ecc = NF_READ_WORD();
        NAND->NFSECCD = (spare_ecc & 0xFF) | ((spare_ecc & 0xFF00) << 8);
        
        if((NAND->NFESTAT0 & 0xF) != 0)
        {
            NF_DESELECT();
            return -1;
        }
    }
    
    NF_DESELECT();

    return 0;
}
#endif

static int debug_init(void)
{
    GPIOH->CON &= ~(0x3 << 4);
    GPIOH->CON |= 0x2 << 4;
    GPIOH->UP |= 0x1 << 2;
    
    UART0->ULCON &= ~0x7F;
    UART0->ULCON |= (UART_PARITY<<3) | (UART_STOP_BIT<<2) | UART_WORD_LEN;

    UART0->UCON &= ~((0x3<<10) | (0x3<<2)); //UART CLOCK = PCLK
    UART0->UCON |= 0x1 << 2;                //Transmit Mode: Interrupt request or polling mode

    float tmp;
    int div;
    
    tmp = (float)(UART_CLOCK)/(UART_BUADRATE*16);
    div = (int)(tmp + 0.5f) - 1;

    UART0->UBRDIV = div;
    
    return 0;
}

void debug(const char *str)
{
    __IO uint8_t *transmit_reg = (__IO uint8_t *)&UART0->UTXH;
    __IO uint8_t *transmit_status_reg = (__IO uint8_t *)&UART0->UTRSTAT;
    
    while('\0' != *str)
    {
        *transmit_reg = *str++;
        while(!(*transmit_status_reg & 0x2));
    }
}

void clean_bss(void)
{
    extern int __bss_start, __bss_end;
    int *ptr = &__bss_start;
    
    for (; ptr < &__bss_end; ptr++)
        *ptr = 0;
}

void sys_init(void)
{
    clean_bss();
    debug_init();
    
    NAND->NFCONF &= ~(0x3<<12) | (0x7<<8) | (0x7<<4);
    NAND->NFCONF |= (NF_TACLS<<12) | (NF_TWRPH0<<8) | (NF_TWRPH1<<4);
    NAND->NFCONT = (NAND->NFCONF & ~0x13) | 0x13;

    NF_SELECT();
    NF_WRITE_CMD(NF_CMD_RESET);
    NF_WAIT_IDLE();
    NF_DESELECT();

    if(0 != sys_nand_read((char *)SDRAM_START_ADDR, CODE_START_ADDR, CODE_SIZE))
    {
        LED_ERR_OFF();
        debug("[S*] Fail to copy code from nand flash to sdram\r\n");
        return;
    }

    debug("Success in sys_init\r\n");
}

