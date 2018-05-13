/*************************************************************************
	> File Name: sys_init.c
	> Author: 
	> Mail: 
	> Created Time: 2018年05月13日 星期日 23时00分00秒
 ************************************************************************/

#include "led.h"
#include "nand_flash.h"

#define CODE_START_ADDR                     4096
#define CODE_SIZE                           (2*NF_PAGE_SIZE)
/*************************************************************************
                     Instruction about CODE_START_ADDR
At least but not last, while using the tools of openjtag, it needs to input
the target block number. But it takes no effect whatever you input. The 
real load address rely on the value in "AT(xxxx)"
 ************************************************************************/

int sys_nand_read(char *target_ptr, uint32_t source_addr, int size);

void sys_init(void)
{
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
        return;
    }
}

int sys_nand_read(char *target_ptr, uint32_t source_addr, int size)
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
