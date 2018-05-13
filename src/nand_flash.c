/*************************************************************************
	> File Name: nand_flash.c
	> Author: 
	> Mail: 
	> Created Time: 2018年05月13日 星期日 23时00分00秒
 ************************************************************************/

#include "nand_flash.h"

int NF_read(char *target_ptr, uint32_t source_addr, int size)
{
    if((source_addr & NF_PAGE_MASK) || (size & NF_PAGE_MASK))
    {
        return -1;
    }

    int i;
    int j;
    uint32_t main_ecc;
    uint32_t spare_ecc;

    size /= NF_PAGE_SIZE;

    NF_SELECT();
    
    for(i = 0; i < size; i++)
    {
        NF_MAIN_ECC_UNLOCK();
        
        NF_WRITE_CMD(NF_CMD_READ_1ST);
        NAND->NFADDR = 0;                               //A0 ~ A7
        NAND->NFADDR = 0;                               //A8 ~ A11
        NAND->NFADDR = (source_addr >> 11) & 0xFF;      //A12 ~ A19
        NAND->NFADDR = (source_addr >> 19) & 0xFF;      //A20 ~ A27
        NAND->NFADDR = (source_addr >> 27) & 0x01;      //A28
        NF_WRITE_CMD(NF_CMD_READ_2ND);
        NF_WAIT_IDLE();

        for(j = 0; j < NF_PAGE_SIZE; j++)
        {
            *target_ptr++ = (uint8_t)NAND->NFDATA;
        }

        source_addr += NF_PAGE_SIZE;

        NF_MAIN_ECC_LOCK();
        NF_SPARE_ECC_UNLOCK();

        main_ecc = NAND->NFDATA;
        main_ecc <<= 8;
        main_ecc = NAND->NFDATA;
        main_ecc <<= 8;
        main_ecc = NAND->NFDATA;
        main_ecc <<= 8;
        main_ecc = NAND->NFDATA;
        
        spare_ecc = NAND->NFDATA;
        spare_ecc <<= 8;
        spare_ecc = NAND->NFDATA;
        spare_ecc <<= 8;
        spare_ecc = NAND->NFDATA;
        spare_ecc <<= 8;
        spare_ecc = NAND->NFDATA;
        
        NAND->NFMECCD0 = (main_ecc & 0xFF) | ((main_ecc & 0xFF00) << 8);
        NAND->NFMECCD1 = ((main_ecc & 0xFF0000) >> 16) | ((main_ecc & 0xFF000000) >> 8);

        NF_SPARE_ECC_LOCK();
        NAND->NFSECCD = (spare_ecc & 0xFF) | ((spare_ecc & 0xFF00) << 8);
        
        if(NAND->NFESTAT0 & 0xF)
        {
            NF_DESELECT();
            return -1;
        }
    }
    
    NF_DESELECT();
    return 0;
}

int NF_write(uint32_t target_addr, char *source_ptr, int size)
{
    return 0;
}

