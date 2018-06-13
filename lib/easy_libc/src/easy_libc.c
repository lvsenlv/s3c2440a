/*************************************************************************
	> File Name: easy_libc.c
	> Author: 
	> Mail: 
	> Created Time: 2018年06月04日 星期一 09时14分23秒
 ************************************************************************/

#include "easy_libc.h"
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

    va_start(args, format_ptr);
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
                       Transplant from glibc-2.3.6
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

size_t easy_strlen(const char *str)
{
    const char *char_ptr;
    const unsigned long int *longword_ptr;
    unsigned long int longword, magic_bits, himagic, lomagic;

    /* Handle the first few characters by reading one character at a time.
       Do this until CHAR_PTR is aligned on a longword boundary.
    */
    for (char_ptr = str; ((unsigned long int) char_ptr & (sizeof (longword) - 1)) != 0; ++char_ptr)
        if (*char_ptr == '\0')
            return char_ptr - str;

    /* All these elucidatory comments refer to 4-byte longwords,
       but the theory applies equally well to 8-byte longwords.
    */

    longword_ptr = (unsigned long int *) char_ptr;

    /* Bits 31, 24, 16, and 8 of this number are zero.  Call these bits
       the "holes."  Note that there is a hole just to the left of
       each byte, with an extra at the end:

       bits:  01111110 11111110 11111110 11111111
       bytes: AAAAAAAA BBBBBBBB CCCCCCCC DDDDDDDD

       The 1-bits make sure that carries propagate to the next 0-bit.
       The 0-bits provide holes for carries to fall into.
    */
    magic_bits = 0x7efefeffL;
    himagic = 0x80808080L;
    lomagic = 0x01010101L;
    if (sizeof (longword) > 4)
    {
        /* 64-bit version of the magic.  */
        /* Do the shift in two steps to avoid a warning if long has 32 bits.  */
        magic_bits = ((0x7efefefeL << 16) << 16) | 0xfefefeffL;
        himagic = ((himagic << 16) << 16) | himagic;
        lomagic = ((lomagic << 16) << 16) | lomagic;
    }
    
    if (sizeof (longword) > 8)
        return 0;

    /* Instead of the traditional loop which tests each character,
       we will test a longword at a time.  The tricky part is testing
       if *any of the four* bytes in the longword in question are zero.
    */
    for (;;)
    {
        /* We tentatively exit the loop if adding MAGIC_BITS to
           LONGWORD fails to change any of the hole bits of LONGWORD.

           1) Is this safe?  Will it catch all the zero bytes?
           Suppose there is a byte with all zeros.  Any carry bits
           propagating from its left will fall into the hole at its
           least significant bit and stop.  Since there will be no
           carry from its most significant bit, the LSB of the
           byte to the left will be unchanged, and the zero will be
           detected.
   
           2) Is this worthwhile?  Will it ignore everything except
           zero bytes?  Suppose every byte of LONGWORD has a bit set
           somewhere.  There will be a carry into bit 8.  If bit 8
           is set, this will carry into bit 16.  If bit 8 is clear,
           one of bits 9-15 must be set, so there will be a carry
           into bit 16.  Similarly, there will be a carry into bit
           24.  If one of bits 24-30 is set, there will be a carry
           into bit 31, so all of the hole bits will be changed.
   
           The one misfire occurs when bits 24-30 are clear and bit
           31 is set; in this case, the hole at bit 31 is not
           changed.  If we had access to the processor carry flag,
           we could close this loophole by putting the fourth hole
           at bit 32!
   
           So it ignores everything except 128's, when they're aligned
           properly.
        */

        longword = *longword_ptr++;

        if (
#if 0
        /* Add MAGIC_BITS to LONGWORD.  */
        (((longword + magic_bits)

        /* Set those bits that were unchanged by the addition.  */
        ^ ~longword)

        /* Look at only the hole bits.  If any of the hole bits
           are unchanged, most likely one of the bytes was a
           zero.
        */
        & ~magic_bits)
#else
        ((longword - lomagic) & himagic)
#endif
        != 0)
        {
            /* Which of the bytes was the zero?  If none of them were, it was
               a misfire; continue the search.
            */

            const char *cp = (const char *) (longword_ptr - 1);

            if (cp[0] == 0)
                return cp - str;
            if (cp[1] == 0)
                return cp - str + 1;
            if (cp[2] == 0)
                return cp - str + 2;
            if (cp[3] == 0)
                return cp - str + 3;
            if (sizeof (longword) > 4)
            {
                if (cp[4] == 0)
                    return cp - str + 4;
                if (cp[5] == 0)
                    return cp - str + 5;
                if (cp[6] == 0)
                    return cp - str + 6;
                if (cp[7] == 0)
                    return cp - str + 7;
            }
        }
    }
}

size_t easy_strnlen(const char *str, size_t maxlen)
{
    const char *char_ptr, *end_ptr = str + maxlen;
    const unsigned long int *longword_ptr;
    unsigned long int longword, magic_bits, himagic, lomagic;

    if (maxlen == 0)
        return 0;

    if (__builtin_expect (end_ptr < str, 0))
        end_ptr = (const char *) ~0UL;

    /* Handle the first few characters by reading one character at a time.
       Do this until CHAR_PTR is aligned on a longword boundary.
    */
    for (char_ptr = str; ((unsigned long int) char_ptr & (sizeof (longword) - 1)) != 0; ++char_ptr)
        if (*char_ptr == '\0')
        {
            if (char_ptr > end_ptr)
                char_ptr = end_ptr;
            return char_ptr - str;
        }

    /* All these elucidatory comments refer to 4-byte longwords,
       but the theory applies equally well to 8-byte longwords.
    */

    longword_ptr = (unsigned long int *) char_ptr;

    /* Bits 31, 24, 16, and 8 of this number are zero.  Call these bits
       the "holes."  Note that there is a hole just to the left of
       each byte, with an extra at the end:

       bits:  01111110 11111110 11111110 11111111
       bytes: AAAAAAAA BBBBBBBB CCCCCCCC DDDDDDDD

       The 1-bits make sure that carries propagate to the next 0-bit.
       The 0-bits provide holes for carries to fall into.
    */
    magic_bits = 0x7efefeffL;
    himagic = 0x80808080L;
    lomagic = 0x01010101L;
    if (sizeof (longword) > 4)
    {
        /* 64-bit version of the magic.  */
        /* Do the shift in two steps to avoid a warning if long has 32 bits.  */
        magic_bits = ((0x7efefefeL << 16) << 16) | 0xfefefeffL;
        himagic = ((himagic << 16) << 16) | himagic;
        lomagic = ((lomagic << 16) << 16) | lomagic;
    }
    
    if (sizeof (longword) > 8)
        return 0;

    /* Instead of the traditional loop which tests each character,
       we will test a longword at a time.  The tricky part is testing
       if *any of the four* bytes in the longword in question are zero.
    */
    while (longword_ptr < (unsigned long int *) end_ptr)
    {
        /* We tentatively exit the loop if adding MAGIC_BITS to
           LONGWORD fails to change any of the hole bits of LONGWORD.

           1) Is this safe?  Will it catch all the zero bytes?
           Suppose there is a byte with all zeros.  Any carry bits
           propagating from its left will fall into the hole at its
           least significant bit and stop.  Since there will be no
           carry from its most significant bit, the LSB of the
           byte to the left will be unchanged, and the zero will be
           detected.

           2) Is this worthwhile?  Will it ignore everything except
           zero bytes?  Suppose every byte of LONGWORD has a bit set
           somewhere.  There will be a carry into bit 8.  If bit 8
           is set, this will carry into bit 16.  If bit 8 is clear,
           one of bits 9-15 must be set, so there will be a carry
           into bit 16.  Similarly, there will be a carry into bit
           24.  If one of bits 24-30 is set, there will be a carry
           into bit 31, so all of the hole bits will be changed.
   
           The one misfire occurs when bits 24-30 are clear and bit
           31 is set; in this case, the hole at bit 31 is not
           changed.  If we had access to the processor carry flag,
           we could close this loophole by putting the fourth hole
           at bit 32!
   
           So it ignores everything except 128's, when they're aligned
           properly.
        */

        longword = *longword_ptr++;

        if ((longword - lomagic) & himagic)
        {
            /* Which of the bytes was the zero?  If none of them were, it was
               a misfire; continue the search.
            */

            const char *cp = (const char *) (longword_ptr - 1);

            char_ptr = cp;
            if (cp[0] == 0)
                break;
            char_ptr = cp + 1;
            if (cp[1] == 0)
                break;
            char_ptr = cp + 2;
            if (cp[2] == 0)
                break;
            char_ptr = cp + 3;
            if (cp[3] == 0)
                break;
            if (sizeof (longword) > 4)
            {
                char_ptr = cp + 4;
                if (cp[4] == 0)
                    break;
                char_ptr = cp + 5;
                if (cp[5] == 0)
                    break;
                char_ptr = cp + 6;
                if (cp[6] == 0)
                    break;
                char_ptr = cp + 7;
                if (cp[7] == 0)
                    break;
            }
        }
        char_ptr = end_ptr;
    }

    if (char_ptr > end_ptr)
        char_ptr = end_ptr;
        
    return char_ptr - str;
}

char *easy_strcpy(char *dest, const char *src)
{
    reg_char c;
    char * s = (char *) CHECK_BOUNDS_LOW (src);
    const ptrdiff_t off = CHECK_BOUNDS_LOW (dest) - s - 1;
    size_t n;
    
    do
    {
        c = *s++;
        s[off] = c;
    }
    while (c != '\0');
    
    n = s - src;
    (void) CHECK_BOUNDS_HIGH (src + n);
    (void) CHECK_BOUNDS_HIGH (dest + n);
    
    return dest;

}

char *easy_strncpy(char *s1, const char *s2, size_t n)
{
    reg_char c;
    char *s = s1;

    --s1;

    if (n >= 4)
    {
        size_t n4 = n >> 2;

        for (;;)
        {
            c = *s2++;
            *++s1 = c;
            if (c == '\0')
                break;
            c = *s2++;
            *++s1 = c;
            if (c == '\0')
                break;
            c = *s2++;
            *++s1 = c;
            if (c == '\0')
                break;
            c = *s2++;
            *++s1 = c;
            if (c == '\0')
                break;
            if (--n4 == 0)
                goto last_chars;
        }
        
        n = n - (s1 - s) - 1;
        if(n == 0)
            return s;
        goto zero_fill;
    }

    last_chars:
    n &= 3;
    if (n == 0)
    return s;

    do
    {
        c = *s2++;
        *++s1 = c;
        if (--n == 0)
            return s;
    }
    while (c != '\0');

    zero_fill:
    do
        *++s1 = '\0';
    while (--n > 0);

    return s;
}

int easy_strcmp(const char *p1, const char *p2)
{
    register const unsigned char *s1 = (const unsigned char *) p1;
    register const unsigned char *s2 = (const unsigned char *) p2;
    unsigned reg_char c1, c2;

    do
    {
        c1 = (unsigned char) *s1++;
        c2 = (unsigned char) *s2++;
        if(c1 == '\0')
            return c1 - c2;
    }while (c1 == c2);

    return c1 - c2;
}

int easy_strncmp(const char *s1, const char *s2, size_t n)
{
    unsigned reg_char c1 = '\0';
    unsigned reg_char c2 = '\0';

    if (n >= 4)
    {
        size_t n4 = n >> 2;
        do
        {
            c1 = (unsigned char) *s1++;
            c2 = (unsigned char) *s2++;
            if (c1 == '\0' || c1 != c2)
                return c1 - c2;
            c1 = (unsigned char) *s1++;
            c2 = (unsigned char) *s2++;
            if (c1 == '\0' || c1 != c2)
                return c1 - c2;
            c1 = (unsigned char) *s1++;
            c2 = (unsigned char) *s2++;
            if (c1 == '\0' || c1 != c2)
                return c1 - c2;
            c1 = (unsigned char) *s1++;
            c2 = (unsigned char) *s2++;
            if (c1 == '\0' || c1 != c2)
                return c1 - c2;
        } while (--n4 > 0);
        n &= 3;
    }

    while (n > 0)
    {
        c1 = (unsigned char) *s1++;
        c2 = (unsigned char) *s2++;
        if (c1 == '\0' || c1 != c2)
            return c1 - c2;
        n--;
    }

    return c1 - c2;
}

char *easy_strcat(char *dest, const char *src)
{
    char *s1 = dest;
    const char *s2 = src;
    reg_char c;

    /* Find the end of the string.  */
    do
        c = *s1++;
    while (c != '\0');

    /* Make S1 point before the next character, so we can increment
       it while memory is read (wins on pipelined cpus).
    */
    s1 -= 2;

    do
    {
        c = *s2++;
        *++s1 = c;
    }
    while (c != '\0');

    return dest;
}

char *easy_strncat(char *s1, const char *s2, size_t n)
{
    reg_char c;
    char *s = s1;

    /* Find the end of S1.  */
    do
        c = *s1++;
    while (c != '\0');

    /* Make S1 point before next character, so we can increment
       it while memory is read (wins on pipelined cpus).
    */
    s1 -= 2;

    if (n >= 4)
    {
        size_t n4 = n >> 2;
        do
        {
            c = *s2++;
            *++s1 = c;
            if (c == '\0')
                return s;
            c = *s2++;
            *++s1 = c;
            if (c == '\0')
                return s;
            c = *s2++;
            *++s1 = c;
            if (c == '\0')
                return s;
            c = *s2++;
            *++s1 = c;
            if (c == '\0')
                return s;
        } while (--n4 > 0);
        n &= 3;
    }

    while (n > 0)
    {
        c = *s2++;
        *++s1 = c;
        if (c == '\0')
            return s;
        n--;
    }

    if (c != '\0')
        *++s1 = '\0';

    return s;
}

char *easy_strstr(const char *phaystack, const char *pneedle)
{
    const unsigned char *haystack, *needle;
    chartype b;
    const unsigned char *rneedle;

    haystack = (const unsigned char *) phaystack;

    if ((b = *(needle = (const unsigned char *) pneedle)))
    {
        chartype c;
        haystack--;		/* possible ANSI violation */

        {
        chartype a;
        do
            if (!(a = *++haystack))
                goto ret0;
        while (a != b);
        }

        if (!(c = *++needle))
            goto foundneedle;
        ++needle;
        goto jin;

        for (;;)
        {
            {
                chartype a;
                if (0)
                jin:
                {
                    if ((a = *++haystack) == c)
                        goto crest;
                }
                else
                    a = *++haystack;
                do
                {
                    for (; a != b; a = *++haystack)
                    {
                        if (!a)
                            goto ret0;
                        if ((a = *++haystack) == b)
                            break;
                        if (!a)
                            goto ret0;
                    }
                }
                while ((a = *++haystack) != c);
	        }
	        
            crest:
            {
                chartype a;
	            {
                const unsigned char *rhaystack;
                if (*(rhaystack = haystack-- + 1) == (a = *(rneedle = needle)))
                do
                {
                    if (!a)
                        goto foundneedle;
                    if (*++rhaystack != (a = *++needle))
                        break;
                    if (!a)
                        goto foundneedle;
                }
                while (*++rhaystack == (a = *++needle));
                needle = rneedle;	/* took the register-poor aproach */
                }
                if (!a)
                    break;
            }
        }
    }
    
foundneedle:
    return (char *) haystack;
    
ret0:
    return 0;
}

#ifdef __LINUX
struct timeval g_StartTime, g_StopTime;

int main(int argc, char **argv)
{
    return 0;
}
#endif //__LINUX





/*************************************************************************
                       Strandard vsnprintf function
 ************************************************************************/

#define NULL                                0
#define UINT32                              unsigned int
#define	UINT16                              unsigned short
#define	UINT8                               unsigned char

#define INT_MAX                             ((int)(~0U>>1))
#define INT_MIN	                            (-INT_MAX - 1)
#define UINT_MAX                            (~0U)
#define LONG_MAX                            ((long)(~0UL>>1))
#define LONG_MIN                            (-LONG_MAX - 1)
#define ULONG_MAX                           (~0UL)

/*
 * NOTE! This ctype does not handle EOF like the standard C
 * library is required to.
 */
#define _U                                  0x01    /* upper */
#define _L                                  0x02    /* lower */
#define _D                                  0x04    /* digit */
#define _C                                  0x08    /* cntrl */
#define _P                                  0x10    /* punct */
#define _S                                  0x20    /* white space (space/lf/tab) */
#define _X                                  0x40    /* hex digit */
#define _SP                                 0x80    /* hard space (0x20) */

unsigned char _ctype[] = {
    _C,_C,_C,_C,_C,_C,_C,_C,                                /* 0-7 */
    _C,_C|_S,_C|_S,_C|_S,_C|_S,_C|_S,_C,_C,                 /* 8-15 */
    _C,_C,_C,_C,_C,_C,_C,_C,                                /* 16-23 */
    _C,_C,_C,_C,_C,_C,_C,_C,                                /* 24-31 */
    _S|_SP,_P,_P,_P,_P,_P,_P,_P,                            /* 32-39 */
    _P,_P,_P,_P,_P,_P,_P,_P,                                /* 40-47 */
    _D,_D,_D,_D,_D,_D,_D,_D,                                /* 48-55 */
    _D,_D,_P,_P,_P,_P,_P,_P,                                /* 56-63 */
    _P,_U|_X,_U|_X,_U|_X,_U|_X,_U|_X,_U|_X,_U,              /* 64-71 */
    _U,_U,_U,_U,_U,_U,_U,_U,                                /* 72-79 */
    _U,_U,_U,_U,_U,_U,_U,_U,                                /* 80-87 */
    _U,_U,_U,_P,_P,_P,_P,_P,                                /* 88-95 */
    _P,_L|_X,_L|_X,_L|_X,_L|_X,_L|_X,_L|_X,_L,              /* 96-103 */
    _L,_L,_L,_L,_L,_L,_L,_L,                                /* 104-111 */
    _L,_L,_L,_L,_L,_L,_L,_L,                                /* 112-119 */
    _L,_L,_L,_P,_P,_P,_P,_C,                                /* 120-127 */
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,                        /* 128-143 */
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,                        /* 144-159 */
    _S|_SP,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,    /* 160-175 */
    _P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,        /* 176-191 */
    _U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,        /* 192-207 */
    _U,_U,_U,_U,_U,_U,_U,_P,_U,_U,_U,_U,_U,_U,_U,_L,        /* 208-223 */
    _L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,        /* 224-239 */
    _L,_L,_L,_L,_L,_L,_L,_P,_L,_L,_L,_L,_L,_L,_L,_L,        /* 240-255 */
};

#define __ismask(x)                         (_ctype[(int)(unsigned char)(x)])

#define isalnum(c)                          ((__ismask(c)&(_U|_L|_D)) != 0)
#define isalpha(c)                          ((__ismask(c)&(_U|_L)) != 0)
#define iscntrl(c)                          ((__ismask(c)&(_C)) != 0)
#define isdigit(c)                          ((__ismask(c)&(_D)) != 0)
#define isgraph(c)                          ((__ismask(c)&(_P|_U|_L|_D)) != 0)
#define islower(c)                          ((__ismask(c)&(_L)) != 0)
#define isprint(c)                          ((__ismask(c)&(_P|_U|_L|_D|_SP)) != 0)
#define ispunct(c)                          ((__ismask(c)&(_P)) != 0)
#define isspace(c)                          ((__ismask(c)&(_S)) != 0)
#define isupper(c)                          ((__ismask(c)&(_U)) != 0)
#define isxdigit(c)                         ((__ismask(c)&(_D|_X)) != 0)

#define isascii(c)                          (((unsigned char)(c))<=0x7f)
#define toascii(c)                          (((unsigned char)(c))&0x7f)

static inline unsigned char __tolower(unsigned char c)
{
    if (isupper(c))
        c -= 'A'-'a';
    return c;
}

static inline unsigned char __toupper(unsigned char c)
{
    if (islower(c))
        c -= 'a'-'A';
    return c;
}

#define tolower(c)                          __tolower(c)
#define toupper(c)                          __toupper(c)

/*
 * The semantics of do_div() are:
 *
 * uint32_t do_div(uint64_t *n, uint32_t base)
 * {
 * 	uint32_t remainder = *n % base;
 * 	*n = *n / base;
 * 	return remainder;
 * }
 *
 * In other words, a 64-bit dividend with a 32-bit divisor producing
 * a 64-bit result and a 32-bit remainder.  To accomplish this optimally
 * we call a special __do_div64 helper with completely non standard
 * calling convention for arguments and results (beware).
 */

#ifdef __ARMEB__
#define __xh "r0"
#define __xl "r1"
#else
#define __xl "r0"
#define __xh "r1"
#endif

#define __asmeq(x, y)                       ".ifnc " x "," y " ; .err ; .endif\n\t"

#define do_div(n,base) \
        ({ \
            register unsigned int __base      asm("r4") = base;	\
            register unsigned long long __n   asm("r0") = n; \
            register unsigned long long __res asm("r2"); \
            register unsigned int __rem       asm(__xh); \
            asm(__asmeq("%0", __xh) \
                __asmeq("%1", "r2") \
                __asmeq("%2", "r0") \
                __asmeq("%3", "r4") \
                "bl	__do_div64" \
                : "=r" (__rem), "=r" (__res) \
                : "r" (__n), "r" (__base) \
                : "ip", "lr", "cc"); \
            n = __res; \
            __rem; \
        })

#define unlikely(x)                         __builtin_expect(!!(x), 0)

/**
 * simple_strtoul - convert a string to an unsigned long
 * @cp: The start of the string
 * @endp: A pointer to the end of the parsed string will be placed here
 * @base: The number base to use
 */
unsigned long simple_strtoul(const char *cp,char **endp,unsigned int base)
{
	unsigned long result = 0,value;

	if (!base) {
		base = 10;
		if (*cp == '0') {
			base = 8;
			cp++;
			if ((toupper(*cp) == 'X') && isxdigit(cp[1])) {
				cp++;
				base = 16;
			}
		}
	} else if (base == 16) {
		if (cp[0] == '0' && toupper(cp[1]) == 'X')
			cp += 2;
	}
	while (isxdigit(*cp) &&
	       (value = isdigit(*cp) ? *cp-'0' : toupper(*cp)-'A'+10) < base) {
		result = result*base + value;
		cp++;
	}
	if (endp)
		*endp = (char *)cp;
	return result;
}

//EXPORT_SYMBOL(simple_strtoul);

/**
 * simple_strtol - convert a string to a signed long
 * @cp: The start of the string
 * @endp: A pointer to the end of the parsed string will be placed here
 * @base: The number base to use
 */
long simple_strtol(const char *cp,char **endp,unsigned int base)
{
	if(*cp=='-')
		return -simple_strtoul(cp+1,endp,base);
	return simple_strtoul(cp,endp,base);
}

//EXPORT_SYMBOL(simple_strtol);

/**
 * simple_strtoull - convert a string to an unsigned long long
 * @cp: The start of the string
 * @endp: A pointer to the end of the parsed string will be placed here
 * @base: The number base to use
 */
unsigned long long simple_strtoull(const char *cp,char **endp,unsigned int base)
{
	unsigned long long result = 0,value;

	if (!base) {
		base = 10;
		if (*cp == '0') {
			base = 8;
			cp++;
			if ((toupper(*cp) == 'X') && isxdigit(cp[1])) {
				cp++;
				base = 16;
			}
		}
	} else if (base == 16) {
		if (cp[0] == '0' && toupper(cp[1]) == 'X')
			cp += 2;
	}
	while (isxdigit(*cp) && (value = isdigit(*cp) ? *cp-'0' : (islower(*cp)
	    ? toupper(*cp) : *cp)-'A'+10) < base) {
		result = result*base + value;
		cp++;
	}
	if (endp)
		*endp = (char *)cp;
	return result;
}

//EXPORT_SYMBOL(simple_strtoull);

/**
 * simple_strtoll - convert a string to a signed long long
 * @cp: The start of the string
 * @endp: A pointer to the end of the parsed string will be placed here
 * @base: The number base to use
 */
long long simple_strtoll(const char *cp,char **endp,unsigned int base)
{
	if(*cp=='-')
		return -simple_strtoull(cp+1,endp,base);
	return simple_strtoull(cp,endp,base);
}

static int skip_atoi(const char **s)
{
	int i=0;

	while (isdigit(**s))
		i = i*10 + *((*s)++) - '0';
	return i;
}

#define ZEROPAD	1		/* pad with zero */
#define SIGN	2		/* unsigned/signed long */
#define PLUS	4		/* show plus */
#define SPACE	8		/* space if plus */
#define LEFT	16		/* left justified */
#define SPECIAL	32		/* 0x */
#define LARGE	64		/* use 'ABCDEF' instead of 'abcdef' */

static char * number(char * buf, char * end, unsigned long long num, int base, int size, int precision, int type)
{
	char c,sign,tmp[66];
	const char *digits;
	static const char small_digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";
	static const char large_digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	int i;

	digits = (type & LARGE) ? large_digits : small_digits;
	if (type & LEFT)
		type &= ~ZEROPAD;
	if (base < 2 || base > 36)
		return NULL;
	c = (type & ZEROPAD) ? '0' : ' ';
	sign = 0;
	if (type & SIGN) {
		if ((signed long long) num < 0) {
			sign = '-';
			num = - (signed long long) num;
			size--;
		} else if (type & PLUS) {
			sign = '+';
			size--;
		} else if (type & SPACE) {
			sign = ' ';
			size--;
		}
	}
	if (type & SPECIAL) {
		if (base == 16)
			size -= 2;
		else if (base == 8)
			size--;
	}
	i = 0;
	if (num == 0)
		tmp[i++]='0';
	else while (num != 0)
		tmp[i++] = digits[do_div(num,base)];
	if (i > precision)
		precision = i;
	size -= precision;
	if (!(type&(ZEROPAD+LEFT))) {
		while(size-->0) {
			if (buf <= end)
				*buf = ' ';
			++buf;
		}
	}
	if (sign) {
		if (buf <= end)
			*buf = sign;
		++buf;
	}
	if (type & SPECIAL) {
		if (base==8) {
			if (buf <= end)
				*buf = '0';
			++buf;
		} else if (base==16) {
			if (buf <= end)
				*buf = '0';
			++buf;
			if (buf <= end)
				*buf = digits[33];
			++buf;
		}
	}
	if (!(type & LEFT)) {
		while (size-- > 0) {
			if (buf <= end)
				*buf = c;
			++buf;
		}
	}
	while (i < precision--) {
		if (buf <= end)
			*buf = '0';
		++buf;
	}
	while (i-- > 0) {
		if (buf <= end)
			*buf = tmp[i];
		++buf;
	}
	while (size-- > 0) {
		if (buf <= end)
			*buf = ' ';
		++buf;
	}
	return buf;
}

/**
 * vsnprintf - Format a string and place it in a buffer
 * @buf: The buffer to place the result into
 * @size: The size of the buffer, including the trailing null space
 * @fmt: The format string to use
 * @args: Arguments for the format string
 *
 * The return value is the number of characters which would
 * be generated for the given input, excluding the trailing
 * '\0', as per ISO C99. If you want to have the exact
 * number of characters written into @buf as return value
 * (not including the trailing '\0'), use vscnprintf. If the
 * return is greater than or equal to @size, the resulting
 * string is truncated.
 *
 * Call this function if you are already dealing with a va_list.
 * You probably want snprintf instead.
 */
int vsnprintf_std(char *buf, size_t size, const char *fmt, va_list args)
{
	int len;
	unsigned long long num;
	int i, base;
	char *str, *end, c;
	const char *s;

	int flags;		/* flags to number() */

	int field_width;	/* width of output field */
	int precision;		/* min. # of digits for integers; max
				   number of chars for from string */
	int qualifier;		/* 'h', 'l', or 'L' for integer fields */
				/* 'z' support added 23/7/1999 S.H.    */
				/* 'z' changed to 'Z' --davidm 1/25/99 */

	/* Reject out-of-range values early */
	if (unlikely((int) size < 0)) {
		/* There can be only one.. */
		static int warn = 1;
//		WARN_ON(warn);
		warn = 0;
		return 0;
	}

	str = buf;
	end = buf + size - 1;

	if (end < buf - 1) {
		end = ((void *) -1);
		size = end - buf + 1;
	}

	for (; *fmt ; ++fmt) {
		if (*fmt != '%') {
			if (str <= end)
				*str = *fmt;
			++str;
			continue;
		}

		/* process flags */
		flags = 0;
		repeat:
			++fmt;		/* this also skips first '%' */
			switch (*fmt) {
				case '-': flags |= LEFT; goto repeat;
				case '+': flags |= PLUS; goto repeat;
				case ' ': flags |= SPACE; goto repeat;
				case '#': flags |= SPECIAL; goto repeat;
				case '0': flags |= ZEROPAD; goto repeat;
			}

		/* get field width */
		field_width = -1;
		if (isdigit(*fmt))
			field_width = skip_atoi(&fmt);
		else if (*fmt == '*') {
			++fmt;
			/* it's the next argument */
			field_width = va_arg(args, int);
			if (field_width < 0) {
				field_width = -field_width;
				flags |= LEFT;
			}
		}

		/* get the precision */
		precision = -1;
		if (*fmt == '.') {
			++fmt;	
			if (isdigit(*fmt))
				precision = skip_atoi(&fmt);
			else if (*fmt == '*') {
				++fmt;
				/* it's the next argument */
				precision = va_arg(args, int);
			}
			if (precision < 0)
				precision = 0;
		}

		/* get the conversion qualifier */
		qualifier = -1;
		if (*fmt == 'h' || *fmt == 'l' || *fmt == 'L' ||
		    *fmt =='Z' || *fmt == 'z') {
			qualifier = *fmt;
			++fmt;
			if (qualifier == 'l' && *fmt == 'l') {
				qualifier = 'L';
				++fmt;
			}
		}

		/* default base */
		base = 10;

		switch (*fmt) {
			case 'c':
				if (!(flags & LEFT)) {
					while (--field_width > 0) {
						if (str <= end)
							*str = ' ';
						++str;
					}
				}
				c = (unsigned char) va_arg(args, int);
				if (str <= end)
					*str = c;
				++str;
				while (--field_width > 0) {
					if (str <= end)
						*str = ' ';
					++str;
				}
				continue;

			case 's':
				s = va_arg(args, char *);
				if ((unsigned long)s < 1024)//PAGE_SIZE)
					s = "<NULL>";

				len = strnlen(s, precision);

				if (!(flags & LEFT)) {
					while (len < field_width--) {
						if (str <= end)
							*str = ' ';
						++str;
					}
				}
				for (i = 0; i < len; ++i) {
					if (str <= end)
						*str = *s;
					++str; ++s;
				}
				while (len < field_width--) {
					if (str <= end)
						*str = ' ';
					++str;
				}
				continue;

			case 'p':
				if (field_width == -1) {
					field_width = 2*sizeof(void *);
					flags |= ZEROPAD;
				}
				str = number(str, end,
						(unsigned long) va_arg(args, void *),
						16, field_width, precision, flags);
				continue;


			case 'n':
				/* FIXME:
				* What does C99 say about the overflow case here? */
				if (qualifier == 'l') {
					long * ip = va_arg(args, long *);
					*ip = (str - buf);
				} else if (qualifier == 'Z' || qualifier == 'z') {
					size_t * ip = va_arg(args, size_t *);
					*ip = (str - buf);
				} else {
					int * ip = va_arg(args, int *);
					*ip = (str - buf);
				}
				continue;

			case '%':
				if (str <= end)
					*str = '%';
				++str;
				continue;

				/* integer number formats - set up the flags and "break" */
			case 'o':
				base = 8;
				break;

			case 'X':
				flags |= LARGE;
			case 'x':
				base = 16;
				break;

			case 'd':
			case 'i':
				flags |= SIGN;
			case 'u':
				break;

			default:
				if (str <= end)
					*str = '%';
				++str;
				if (*fmt) {
					if (str <= end)
						*str = *fmt;
					++str;
				} else {
					--fmt;
				}
				continue;
		}
		if (qualifier == 'L')
			num = va_arg(args, long long);
		else if (qualifier == 'l') {
			num = va_arg(args, unsigned long);
			if (flags & SIGN)
				num = (signed long) num;
		} else if (qualifier == 'Z' || qualifier == 'z') {
			num = va_arg(args, size_t);
		} else if (qualifier == 'h') {
			num = (unsigned short) va_arg(args, int);
			if (flags & SIGN)
				num = (signed short) num;
		} else {
			num = va_arg(args, unsigned int);
			if (flags & SIGN)
				num = (signed int) num;
		}
		str = number(str, end, num, base,
				field_width, precision, flags);
	}
	if (str <= end)
		*str = '\0';
	else if (size > 0)
		/* don't write out a null byte if the buf size is zero */
		*end = '\0';
	/* the trailing null byte doesn't count towards the total
	* ++str;
	*/
	return str-buf;
}

