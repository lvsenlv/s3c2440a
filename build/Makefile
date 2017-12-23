TARGET = s3c2440a
ROOT_DIR = $(shell cd ../ && pwd)
INCS = -I$(ROOT_DIR)/include
BUILD_DIR = $(ROOT_DIR)/build
OBJS_DIR = $(ROOT_DIR)/build/object
SRC_DIR = $(ROOT_DIR)/src

OBJS = system_init.o led.o main.o

CC := arm-linux-gcc
OBJ_DUMP := arm-linux-objdump
OBJ_COPY := arm-linux-objcopy
LD := arm-linux-ld
CFLAGS := $(INCS) -g -Wall -Wno-strict-aliasing -O0
LIBS :=
$(shell mkdir -p $(OBJS_DIR))

all : make_start_up $(TARGET)

make_start_up :
	$(CC) -c $(SRC_DIR)/startup.s -o $(BUILD_DIR)/startup.o 

$(TARGET) : $(OBJS)
	$(LD) -Ttext 0x00000000 $(BUILD_DIR)/startup.o $(wildcard $(OBJS_DIR)/*.o) -o $@
	$(OBJ_COPY) -O binary -S $@ $@.bin
	$(OBJ_DUMP) -D -m arm $@ > $@.dis

%.o : $(SRC_DIR)/%.c
	$(CC) -c $< -o $(OBJS_DIR)/$@ $(CFLAGS) $(LIBS)

.PHONY : clean
	
clean :
	rm -rf $(OBJS_DIR) $(TARGET)* *.o
