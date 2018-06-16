/*************************************************************************
	> File Name: easy_libc.h
	> Author: 
	> Mail: 
	> Created Time: 2018年06月04日 星期一 09时14分36秒
 ************************************************************************/

#ifndef __EASY_LIBC_H
#define __EASY_LIBC_H

//#define __STD_PRINTF
#define __STD_STRING

#ifndef HEX_SYMBOL_UPPERCASE
#define HEX_SYMBOL_LOWERCASE
#endif

#define printf                              easy_printf

#ifdef __STD_PRINTF
#define vsnprintf                           vsnprintf_std
#else
#define vsnprintf                           easy_vsnprintf
#endif

#ifdef __STD_STRING
#define memset                              easy_memset
#define strlen                              easy_strlen
#define strnlen                             easy_strnlen
#define strcpy                              easy_strcpy
#define strncpy                             easy_strncpy
#define strcmp                              easy_strcmp
#define strncmp                             easy_strncmp
#define strcat                              easy_strcat
#define strncat                             easy_strncat
#endif

typedef unsigned int                        size_t;
typedef char *                              va_list;
#ifndef NULL
#define NULL                                ((void *)0)
#endif

//va_list related >>>
#define	NATIVE_INT                          int
#define _AUPBND                             (sizeof (NATIVE_INT) - 1)
#define _ADNBND                             (sizeof (NATIVE_INT) - 1)
#define _bnd(X, bnd)                        (((sizeof (X)) + (bnd)) & (~(bnd)))
#define va_arg(ap, T)                       (*(T *)(((ap) += (_bnd (T, _AUPBND))) - (_bnd (T,_ADNBND))))
#define va_end(ap)                          ((void)0)
#define va_start(ap, A)                     (void) ((ap) = (((char *) &(A)) + (_bnd (A,_AUPBND))))
//va_list related <<<

int int_to_strn(char *buf_ptr, int buf_size, int data);
int int_to_hex_strn(char *buf_ptr, int buf_size, unsigned int data);
int easy_printf(const char *format_ptr, ...) __attribute__((__format__ (__printf__, 1, 2)));
int easy_vsnprintf(char *buf_ptr, int buf_size, const char *format_ptr, va_list args);
void *easy_memset(void *dstpp, int c, size_t len);
size_t easy_strlen(const char *str);
size_t easy_strnlen(const char *str, size_t maxlen);
char *easy_strcpy(char *dest, const char *src);
char *easy_strncpy(char *s1, const char *s2, size_t n);
int easy_strcmp(const char *p1, const char *p2);
int easy_strncmp(const char *s1, const char *s2, size_t n);
char *easy_strcat(char *dest, const char *src);
char *easy_strncat(char *s1, const char *s2, size_t n);
int vsnprintf_std(char *buf, size_t size, const char *fmt, va_list args);

extern void uart_send_str(const char *str);

#endif
