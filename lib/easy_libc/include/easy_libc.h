/*************************************************************************
	> File Name: easy_libc.h
	> Author: 
	> Mail: 
	> Created Time: 2018年06月04日 星期一 09时14分36秒
 ************************************************************************/

#ifndef __EASY_LIBC_H
#define __EASY_LIBC_H

#ifdef __LINUX
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <stdarg.h>

typedef unsigned long int                   op_t;
typedef unsigned char                       byte;
typedef unsigned                            chartype;
typedef long int                            ptrdiff_t;
#define OPSIZ                               (sizeof(op_t))
#define	reg_char                            char
#define CHECK_BOUNDS_LOW(ARG)               (ARG)
#define CHECK_BOUNDS_HIGH(ARG)              (ARG)

extern struct timeval g_StartTime, g_StopTime;
    
#define START_COUNT                     gettimeofday(&g_StartTime, NULL)
#define STOP_COUNT                      gettimeofday(&g_StopTime, NULL)

#ifdef __REDIRECTION
#define GET_TIME \
        fprintf(stdout, "projec cost about %ldus in total\n", \
                (g_StopTime.tv_sec - g_StartTime.tv_sec) * 1000000 + \
                g_StopTime.tv_usec - g_StartTime.tv_usec)
#else //REDIRECTION
#define GET_TIME \
        fprintf(stdout, "projec cost about %ldus in total\n", \
                (g_StopTime.tv_sec - g_StartTime.tv_sec) * 1000000 + \
                g_StopTime.tv_usec - g_StartTime.tv_usec)
#endif //__REDIRECTION
#endif



//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

#ifndef __LINUX

//Transplant from glibc-2.3.6 >>>
typedef unsigned int                        size_t;
typedef unsigned long int                   op_t;
typedef unsigned char                       byte;
typedef unsigned                            chartype;
typedef long int                            ptrdiff_t;
#define OPSIZ                               (sizeof(op_t))
#define	reg_char                            char
#define CHECK_BOUNDS_LOW(ARG)               (ARG)
#define CHECK_BOUNDS_HIGH(ARG)              (ARG)

//Transplant from glibc-2.3.6 <<<

#ifndef HEX_SYMBOL_UPPERCASE
#define HEX_SYMBOL_LOWERCASE
#endif

#ifndef NULL
#define NULL                                ((void *)0)
#endif

typedef char *va_list;

/*
    Storage alignment properties
*/
#define	 NATIVE_INT		 int
#define  _AUPBND         (sizeof (NATIVE_INT) - 1)
#define  _ADNBND         (sizeof (NATIVE_INT) - 1)

/*
    Variable argument list macro definitions
*/
#define _bnd(X, bnd)    (((sizeof (X)) + (bnd)) & (~(bnd)))
#define va_arg(ap, T)   (*(T *)(((ap) += (_bnd (T, _AUPBND))) - (_bnd (T,_ADNBND))))
#define va_end(ap)      (void) 0
#define va_start(ap, A) (void) ((ap) = (((char *) &(A)) + (_bnd (A,_AUPBND))))

#define printf                              easy_printf
#define vsnprintf                           easy_vsnprintf
#define memset                              easy_memset
#define strlen                              easy_strlen
#define strnlen                             easy_strnlen
#define strcpy                              easy_strcpy
#define strncpy                             easy_strncpy
#define strcmp                              easy_strcmp
#define strncmp                             easy_strncmp
#define strcat                              easy_strcat
#define strncat                             easy_strncat

#endif //#ifndef __LINUX



//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


int int_to_strn(char *buf_ptr, int buf_size, int data);
//int easy_printf(const char *format_ptr, ...) __attribute__((__format__ (__printf__, 1, 2)));
int easy_printf(const char *format_ptr, ...);
int easy_vsnprintf(char *buf_ptr, int buf_size, const char *format_ptr, va_list args);
void *easy_memset(void *dstpp, int c, size_t len);

#endif
