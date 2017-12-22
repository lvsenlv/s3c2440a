/*************************************************************************
	> File Name: system_init.c
	> Author: 
	> Mail: 
	> Created Time: 2017年12月23日 星期六 00时26分33秒
 ************************************************************************/

void system_init(void)
{
    unsigned long *ptr;
    ptr = (unsigned long *)0x53000000;
    *ptr &= ~(0x1 << 5);
}
