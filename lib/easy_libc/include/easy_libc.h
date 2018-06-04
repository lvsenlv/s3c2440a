/*************************************************************************
	> File Name: easy_libc.h
	> Author: 
	> Mail: 
	> Created Time: 2018年06月04日 星期一 09时14分36秒
 ************************************************************************/

#ifndef __EASY_LIBC_H
#define __EASY_LIBC_H



/*************************************************************************
                        va_list related function
 ************************************************************************/
#include <stdarg.h>
#if 0
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
#endif



/*************************************************************************
                        string related function
 ************************************************************************/

int eprintf(const char *format_ptr, ...) __attribute__((__format__ (__printf__, 1, 2)));
int evsnprintf(char *buf_ptr, int buf_size, const char *format_ptr, va_list args);
int int_to_strn(char *buf_ptr, int buf_size, int data);
int int_to_strn_align(char *buf_ptr, int buf_size, int data, int align);
int int_to_hex_strn(char *buf_ptr, int buf_size, int data);
int int_to_hex_strn_align(char *buf_ptr, int buf_size, unsigned int data, int align);

#endif
