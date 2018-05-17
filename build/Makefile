TARGET_BASE_NAME    = s3c2440a
CC                  = arm-linux-gcc
LD                  = arm-linux-ld
AR                  = arm-linux-ar
OBJCOPY             = arm-linux-objcopy
OBJDUMP             = arm-linux-objdump
CFLAGS              = -Wall -Werror -O3
CPPFLAGS            = -nostdinc -fno-builtin
SRC_DIR             = ../src
INC_DIR             = ../include
OBJ_DIR             = ./object
LIB_DIR             = ../lib
LIBS                = -L../lib/libc/build/ -lc
OBJS                = main.o nand_flash.o uart.o
DEF                 = -D__REDIRECTION
INC                 = -I../include
TARGET              = $(TARGET_BASE_NAME)

$(shell mkdir -p $(OBJ_DIR))

all : start_up $(TARGET)

start_up :
	$(CC) -c ../start_up/$@.s -o ../start_up/$@.o -O0
	$(CC) -c ../start_up/sys_init.c -o ../start_up/sys_init.o $(INC) -Wall -Werror -O3

$(TARGET) : before $(OBJS)
	$(LD) -Tlds $(wildcard $(OBJ_DIR)/*.o) $(LIBS) -o $@
	$(OBJCOPY) -O binary -S $@ $@.bin
	$(OBJDUMP) -D -m arm $@ > dis

before : libc.a
ifeq ($(wildcard $(LIB_DIR)/libc/build/libc.a),)
libc.a :
	$(MAKE) -C $(LIB_DIR)/libc/build DEF="$(DEF)"
	cp $(LIB_DIR)/libc/include/printf.h $(INC_DIR)
else
libc.a :
endif

%.o : $(SRC_DIR)/%.c
	$(CC) -c $<  -o $(OBJ_DIR)/$@ $(DEF) $(INC) $(CFLAGS) 

fast :
	gcc fast.c -o fast $(CFLAGS)

clean_all : clean clean_lib

clean :
	rm -rf $(OBJ_DIR) $(TARGET_BASE_NAME)* ../start_up/*.o dis fast

clean_lib :
	rm -f $(LIB_DIR)/*.a $(LIB_DIR)/*.h
	$(MAKE) -C $(LIB_DIR)/libc/build clean

