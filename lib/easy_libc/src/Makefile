all :
	gcc easy_libc.c -I ../include -Wall -O3 -fno-builtin -D__LINUX

arm:
	arm-linux-gcc easy_libc.c -D__ARM -I ../include -Wall -O0 -fno-builtin
	arm-linux-objdump -S a.out > dis

clean :
	rm -f dis *.s *.o a.out
