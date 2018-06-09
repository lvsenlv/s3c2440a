/*************************************************************************
	> File Name: sys_init.c
	> Author: 
	> Mail: 
	> Created Time: 2018年05月13日 星期日 23时00分00秒
 ************************************************************************/

#include "led.h"
#include "nand_flash.h"
#include "uart.h"

#define CODE_START_ADDR                     4096
#define CODE_SIZE                           (2*NF_PAGE_SIZE)
/*************************************************************************
                     Instruction about CODE_START_ADDR
At least but not last, while using the tools of openjtag, it needs to input
the target block number. But it takes no effect whatever you input. The 
real load address rely on the value in "AT(xxxx)"
 ************************************************************************/

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
    int *p = &__bss_start;
    
    for (; p < &__bss_end; p++)
        *p = 0;
}

void sys_init(void)
{
    debug_init();
    clean_bss();
    
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
        debug("[Fatal Error *]Fail to copy code from nand flash to sdram\r\n");
        return;
    }

}

