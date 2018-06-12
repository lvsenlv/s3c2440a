/*************************************************************************
	> File Name: easy_libc.c
	> Author: 
	> Mail: 
	> Created Time: 2018年06月04日 星期一 09时14分23秒
 ************************************************************************/

#include "easy_libc.h"

#ifdef __LINUX
struct timeval g_StartTime, g_StopTime;

int main(void)
{
    char buf[1024];
    int i;
    long time = 0;

    for(i = 0; i < 0xFFFF; i++)
    {
        START_COUNT;
        memset(buf, 0, sizeof(buf));
        STOP_COUNT;

        time += (g_StopTime.tv_sec - g_StartTime.tv_sec) * 1000000 + 
                g_StopTime.tv_usec - g_StartTime.tv_usec;
    }

    printf("Total Time: %ldus, Average time: %fus\n", time, (float)time/0xFFFF);
    
    return 0;
}
#endif //__LINUX



int int_to_strn(char *buf_ptr, int buf_size, int data)
{
    char *cur_ptr;
    char *end_ptr;
    char buf[12]; //12: 1 + 10 + 1 = Symbol + int_string_max_len + '\0'
    int i;

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

    i--;
    while((cur_ptr < end_ptr) && (i >= 0))
    {
        *cur_ptr++ = buf[i--];
    }
    
    return cur_ptr-buf_ptr;
}

int int_to_hex_strn(char *buf_ptr, int buf_size, unsigned int data)
{
    char *cur_ptr;
    char *end_ptr;
    char buf[8];
    int i;
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
        tmp = data & 0xF;
#ifdef HEX_SYMBOL_UPPERCASE
        buf[i++] = (10 > tmp) ? (tmp + '0') : (tmp - 10 + 'A');
#else
        buf[i++] = (10 > tmp) ? (tmp + '0') : (tmp - 10 + 'a');
#endif
        data >>= 4;
    }

    i--;
    while((cur_ptr < end_ptr) && (i >= 0))
    {
        *cur_ptr++ = buf[i--];
    }
    
    return cur_ptr-buf_ptr;
}

int easy_printf(const char *format_ptr, ...)
{
    static char buf[1024];
    va_list args;

    va_start(format_ptr, args);
    vsnprintf(buf, sizeof(buf), format_ptr, args);
    va_end(args);
    
    return 0;
}

/*
    Only support for following expression:
    %c    %d    %p    %s    %x
*/
int easy_vsnprintf(char *buf_ptr, int buf_size, const char *format_ptr, va_list args)
{
    char *cur_ptr;
    char *end_ptr;
    char *tmp_ptr;
    int res;

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

        switch(*format_ptr)
        {
            case 'c':
                *cur_ptr++ = (char)va_arg(args, int);
                break;
            case 'd':
                res = int_to_strn(cur_ptr, end_ptr-cur_ptr, va_arg(args, int));
                cur_ptr += res;
                
                break;
            case 'p':
                if((cur_ptr+2) >= end_ptr)
                    break;
                    
                *cur_ptr++ = '0';
                *cur_ptr++ = 'x';
                res = int_to_hex_strn(cur_ptr, end_ptr-cur_ptr, (unsigned int)va_arg(args, long));
                cur_ptr += res;
                break;
            case 's':
                tmp_ptr = va_arg(args, char *);
                while('\0' != *tmp_ptr)
                {
                    *cur_ptr++ = *tmp_ptr++;
                    if(cur_ptr >= end_ptr)
                        break;
                }
                break;
            case 'x':
                res = int_to_hex_strn(cur_ptr, end_ptr-cur_ptr, va_arg(args, int));
                cur_ptr += res;
                break;
            default:
                if('%' == format_ptr[1])
                {
                    if((cur_ptr+2) < end_ptr)
                    {
                        *cur_ptr++ = '%';
                        *cur_ptr++ = '%';
                    }
                    break;
                }

                //strcpy(buf_ptr, "[S*] Invalid format");
                break;
        }

        format_ptr++;
    }

    *cur_ptr = '\0';
    
    return cur_ptr-buf_ptr;
}



/*************************************************************************
                        string related function
 ************************************************************************/
void *easy_memset(void *dstpp, int c, size_t len)
{
    long int dstp = (long int) dstpp;

    if (len >= 8)
    {
        size_t xlen;
        op_t cccc;

        cccc = (unsigned char) c;
        cccc |= cccc << 8;
        cccc |= cccc << 16;
        if (OPSIZ > 4)
            /* Do the shift in two steps to avoid warning if long has 32 bits.  */
            cccc |= (cccc << 16) << 16;

        /* There are at least some bytes to set.
        No need to test for LEN == 0 in this alignment loop.  */
        while (dstp % OPSIZ != 0)
        {
            ((byte *) dstp)[0] = c;
            dstp += 1;
            len -= 1;
        }

        /* Write 8 `op_t' per iteration until less than 8 `op_t' remain.  */
        xlen = len / (OPSIZ * 8);
        while (xlen > 0)
        {
            ((op_t *) dstp)[0] = cccc;
            ((op_t *) dstp)[1] = cccc;
            ((op_t *) dstp)[2] = cccc;
            ((op_t *) dstp)[3] = cccc;
            ((op_t *) dstp)[4] = cccc;
            ((op_t *) dstp)[5] = cccc;
            ((op_t *) dstp)[6] = cccc;
            ((op_t *) dstp)[7] = cccc;
            dstp += 8 * OPSIZ;
            xlen -= 1;
        }
        len %= OPSIZ * 8;

        /* Write 1 `op_t' per iteration until less than OPSIZ bytes remain.  */
        xlen = len / OPSIZ;
        while (xlen > 0)
        {
            ((op_t *) dstp)[0] = cccc;
            dstp += OPSIZ;
            xlen -= 1;
        }
        len %= OPSIZ;
    }

    /* Write the last few bytes.  */
    while (len > 0)
    {
        ((byte *) dstp)[0] = c;
        dstp += 1;
        len -= 1;
    }

    return dstpp;
}
