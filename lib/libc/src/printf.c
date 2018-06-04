#include "vsprintf.h"
#include "string.h"
#include "printf.h"

#ifndef __REDIRECTION
extern void putc(unsigned char c);
#endif
extern unsigned char getc(void);

#define	OUTBUFSIZE	1024
#define	INBUFSIZE	1024

static unsigned char g_pcOutBuf[OUTBUFSIZE];
static unsigned char g_pcInBuf[INBUFSIZE];

int printf(const char *fmt, ...)
{
	int i;
	int len;
	va_list args;

#ifdef __REDIRECTION
    volatile unsigned char *uart_utxh0;
    volatile unsigned char *uart_utrstat0;

#warning "------------------------------------------"
#warning "    UART->UTXH0 Address: 0x50000020       "
#warning "    UART->UTRSTAT0 Address: 0x50000010    "
#warning "------------------------------------------"

    uart_utxh0 = (volatile unsigned char *)0x50000020;
    uart_utrstat0 = (volatile unsigned char *)0x50000010;
#endif

	va_start(args, fmt);
	len = vsprintf((char *)g_pcOutBuf,fmt,args);
	va_end(args);

	for (i = 0; '\0' != g_pcOutBuf[i]; i++)
	{
#ifdef __REDIRECTION
		*uart_utxh0 = g_pcOutBuf[i];
        while(!(*uart_utrstat0 & 0x2));
#else
        putc(g_pcOutBuf[i]);
#endif
	}
	return len;
}



int scanf(const char * fmt, ...)
{
	int i = 0;
	unsigned char c;
	va_list args;
	
	while(1)
	{
		c = getc();
		if((c == 0x0d) || (c == 0x0a))
		{
			g_pcInBuf[i] = '\0';
			break;
		}
		else
		{
			g_pcInBuf[i++] = c;
		}
	}
	
	va_start(args,fmt);
	i = vsscanf((char *)g_pcInBuf,fmt,args);
	va_end(args);

	return i;
}

