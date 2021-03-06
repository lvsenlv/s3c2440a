#include "vsprintf.h"
#include "printf.h"
#include "string.h"

extern void putc(unsigned char c);
extern unsigned char getc(void);

#define	OUTBUFSIZE	1024
#define	INBUFSIZE	1024


static unsigned char g_pcOutBuf[OUTBUFSIZE];
static unsigned char g_pcInBuf[INBUFSIZE];

void putc(unsigned char ch)
{
#ifdef __REDIRECTION
    volatile unsigned char *uart_utxh0;
    volatile unsigned char *uart_utrstat0;

#warning "------------------------------------------"
#warning "    UART->UTXH0 Address: 0x50000020       "
#warning "    UART->UTRSTAT0 Address: 0x50000010    "
#warning "------------------------------------------"

    uart_utxh0 = (volatile unsigned char *)0x50000020;
    uart_utrstat0 = (volatile unsigned char *)0x50000010;

    *uart_utxh0 = ch;
    while(!(*uart_utrstat0 & 0x2));
#endif
}


int printf(const char *fmt, ...)
{
	int i;
	int len;
	va_list args;

	va_start(args, fmt);
	len = vsprintf(g_pcOutBuf,fmt,args);
	va_end(args);
	for (i = 0; i < strlen(g_pcOutBuf); i++)
	{
        putc(g_pcOutBuf[i]);
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
	i = vsscanf(g_pcInBuf,fmt,args);
	va_end(args);

	return i;
}

