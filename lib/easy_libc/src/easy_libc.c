/*************************************************************************
	> File Name: easy_libc.c
	> Author: 
	> Mail: 
	> Created Time: 2018年06月04日 星期一 09时14分23秒
 ************************************************************************/

#include "easy_libc.h"
#include <stdio.h>

int main(void)
{
    eprintf("0x%x\n", 0xABCDEF98);
    
    return 0;
}

int eprintf(const char *format_ptr, ...)
{
    va_list args;
    char buf[1024];

    va_start(args, format_ptr);
    evsnprintf(buf, sizeof(buf), format_ptr, args);
    va_end(args);

    printf("%s", buf);

    return 0;
}

/*
    Support following expression:
    %d    %x    %p
*/
int evsnprintf(char *buf_ptr, int buf_size, const char *format_ptr, va_list args)
{
    char *cur_ptr;
    char *end_ptr;
    int res;
    int align;
    char flag;

    cur_ptr = buf_ptr;
    end_ptr = buf_ptr + buf_size - 1;

    while((cur_ptr < end_ptr) && ('\0' != *format_ptr))
    {
        if('%' != *format_ptr)
        {
            *cur_ptr++ = *format_ptr++;
            continue;
        }

        //Tackle % format
        format_ptr++;
        flag = 0;

        while(('\0' != *format_ptr) && ('0' == *format_ptr))
        {
            format_ptr++;
            flag = 1;
        }

        align = 0;
        
        while(('0' <= *format_ptr) && ('9' >= *format_ptr))
        {
            align *= 10;
            align += *format_ptr - '0';
            format_ptr++;
        }

        if(0 == flag)
        {
            align |= 0x1 << 16; //Align symbol is ' ' instead of '0'
        }

        if('d' == *format_ptr)
        {
            res = int_to_strn_align(cur_ptr, end_ptr-cur_ptr, va_arg(args, int), align);
            if(0 > res)
                break;

            cur_ptr += res;
        }
        else if('x' == *format_ptr)
        {
            res = int_to_hex_strn_align(cur_ptr, end_ptr-cur_ptr, va_arg(args, int), align);
            if(0 > res)
                break;

            cur_ptr += res;
        }
        else
        {
            *cur_ptr++ = '%';
        }

        format_ptr++;
        continue;
    }

    *cur_ptr = '\0';
    
    return cur_ptr-buf_ptr;
}

int int_to_strn(char *buf_ptr, int buf_size, int data)
{
    return int_to_strn_align(buf_ptr, buf_size, data, 0);
}

int int_to_hex_strn(char *buf_ptr, int buf_size, int data)
{
    return int_to_hex_strn_align(buf_ptr, buf_size, data, 0);
}


int int_to_strn_align(char *buf_ptr, int buf_size, int data, int align)
{
    char *cur_ptr;
    char *end_ptr;
    char buf[1024];
    int i;
    int j;
    char align_symbol;

    if(0 >= buf_size)
        return 0;

    cur_ptr = buf_ptr;
    end_ptr = buf_ptr + buf_size;
    i = 0;
    
    if(0 == data)
    {
        buf[i++] = '0';
    }
    else if(0 > data)
    {
        *cur_ptr++ = '-';
        data *= -1;
    }

    while(0 != data)
    {
        buf[i++] = data % 10 + '0';
        data = data / 10;
    }

    align_symbol = (align&0xFF0000) ? ' ' : '0';

    align &= 0xFFFF;
    if((buf_size-i) < align)
    {
        align = 0;
    }
    
    if(align > i)
    {
        j = align - i;
        for(; j > 0; j--)
        {
            buf[i++] = align_symbol;
        }
    }

    i--;
    while((cur_ptr < end_ptr) && (i >= 0))
    {
        *cur_ptr++ = buf[i--];
    }
    
    return cur_ptr-buf_ptr;
}

int int_to_hex_strn_align(char *buf_ptr, int buf_size, unsigned int data, int align)
{
    char *cur_ptr;
    char *end_ptr;
    char buf[1024];
    int i;
    int j;
    char align_symbol;
    int tmp;

    if(0 >= buf_size)
        return 0;

    cur_ptr = buf_ptr;
    end_ptr = buf_ptr + buf_size;
    i = 0;
    
    if(0 == data)
    {
        buf[i++] = '0';
    }

    while(0 != data)
    {
        tmp = data % 16;
        buf[i++] = (10 > tmp) ? (tmp + '0') : (tmp - 10 + 'A');
        data /= 16;
    }

    align_symbol = (align&0xFF0000) ? ' ' : '0';

    align &= 0xFFFF;
    if((buf_size-i) < align)
    {
        align = 0;
    }
    
    if(align > i)
    {
        j = align - i;
        for(; j > 0; j--)
        {
            buf[i++] = align_symbol;
        }
    }

    i--;
    while((cur_ptr < end_ptr) && (i >= 0))
    {
        *cur_ptr++ = buf[i--];
    }
    
    return cur_ptr-buf_ptr;
}
