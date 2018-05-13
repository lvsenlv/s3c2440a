TARGET_BASE_NAME    = s3c2440a
CC                  = arm-linux-gcc
LD                  = arm-linux-ld
OBJCOPY             = arm-linux-objcopy
OBJDUMP             = arm-linux-objdump
LIBS                = 
CFLAGS              = -Wall -Werror -O3
SRC_DIR             = ../src
OBJ_DIR             = ./object
LIB_DIR             = ../lib
OBJS                = main.o nand_flash.o
DEF                 =  
INC                 = -I../include 
TARGET              = $(TARGET_BASE_NAME)

$(shell mkdir -p $(OBJ_DIR))

all : start_up $(TARGET)

start_up :
	$(CC) -c ../start_up/$@.s -o ../start_up/$@.o -O0
	$(CC) -c ../start_up/sys_init.c -o ../start_up/sys_init.o $(INC) -Wall -Werror -O3

$(TARGET) : $(OBJS)
	$(LD) -Tlds $(wildcard $(OBJ_DIR)/*.o) -o $@
	$(OBJCOPY) -O binary -S $@ $@.bin
	$(OBJDUMP) -D -m arm $@ > dis

%.o : $(SRC_DIR)/%.c
	$(CC) -c $<  -o $(OBJ_DIR)/$@ $(DEF) $(INC) $(CFLAGS) 

fast :
	gcc fast.c -o fast $(CFLAGS)

clean :
	rm -rf $(TARGET_DIR) $(OBJ_DIR) $(TARGET_BASE_NAME)* ../start_up/*.o dis fast
