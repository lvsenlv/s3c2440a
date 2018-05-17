
#ifndef _PRINTF_H
#define _PRINTF_H

int printf(const char *fmt, ...) __attribute__((__format__ (__printf__, 1, 2)));
int scanf(const char * fmt, ...);

#endif /* _PRINTF_H */
