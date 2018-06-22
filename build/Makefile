TARGET_BASE_NAME    = s3c2440a
CC                  = arm-linux-gcc
LD                  = arm-linux-ld
AR                  = arm-linux-ar
OBJCOPY             = arm-linux-objcopy
OBJDUMP             = arm-linux-objdump
CFLAGS              = -Wall -Werror -nostdinc -O3
#CFLAGS              = -Wall -Werror -nostdinc -fno-builtin -O3
SRC_DIR             = ../src
INC_DIR             = ../include
OBJ_DIR             = ./object
LIB_DIR             = ../lib
LIBS                = -L$(LIB_DIR) -leasy_libc
OBJS                = main.o nand_flash.o uart.o interrupt.o
DEF                 = -D__ARM -D__REDIRECTION
INC                 = -I../include -I../lib/easy_libc/include -I../lib/libc/include
TARGET              = $(TARGET_BASE_NAME)

$(shell mkdir -p $(OBJ_DIR))

all : start_up $(TARGET)

start_up :
	$(CC) -c ../start_up/$@.s -o ../start_up/$@.o -O0 -nostdinc
	$(CC) -c ../start_up/sys_init.c -o ../start_up/sys_init.o $(INC) -Wall -Werror -O3 -nostdinc

$(TARGET) : lib_dep $(OBJS)
	$(LD) -Tlds $(wildcard $(OBJ_DIR)/*.o) $(LIBS) -o $@
	$(OBJCOPY) -O binary -S $@ $@.bin
	$(OBJDUMP) -D -m arm $@ > dis

%.o : $(SRC_DIR)/%.c
	$(CC) -c $<  -o $(OBJ_DIR)/$@ $(DEF) $(INC) $(CFLAGS) 

fast :
	gcc fast.c -o fast $(CFLAGS)

clean_all : clean clean_lib

clean :
	rm -rf $(OBJ_DIR) $(TARGET_BASE_NAME)* ../start_up/*.o dis fast

lib_dep : easy_libc 

easy_libc :
	$(MAKE) -C $(LIB_DIR)/$@/build DEF="$(DEF)"

libc :
	$(MAKE) -C $(LIB_DIR)/$@/build DEF="$(DEF)"

clean_lib :
	$(MAKE) -C $(LIB_DIR)/easy_libc/build clean
	$(MAKE) -C $(LIB_DIR)/libc/build clean

