TARGET = s3c2440a
ROOT_DIR = $(shell cd ../ && pwd)
BUILD_DIR = $(ROOT_DIR)/build
OBJS_DIR = $(ROOT_DIR)/build/object
SRC_DIR = $(ROOT_DIR)/src
CROSS_COMPILE_TOOL_DIR = /usr/local/gcc-3.4.5-glibc-2.3.6

OBJS = led.o uart.o main.o

CC := arm-linux-gcc
OBJ_DUMP := arm-linux-objdump
OBJ_COPY := arm-linux-objcopy
LD := arm-linux-ld
INCS := -I$(ROOT_DIR)/include
CFLAGS := $(INCS) -Wall -Wno-strict-aliasing -O2
LIBS := -L$(CROSS_COMPILE_TOOL_DIR)/lib \
		-L$(CROSS_COMPILE_TOOL_DIR)/arm-linux/lib \
		-L$(CROSS_COMPILE_TOOL_DIR)/lib/gcc/arm-linux/3.4.5 \
		-lgcc
$(shell mkdir -p $(OBJS_DIR))

all : before $(TARGET)

before :
	$(CC) -c $(SRC_DIR)/startup.s -o $(BUILD_DIR)/startup.o
	$(CC) -c $(SRC_DIR)/system_init.c -o $(BUILD_DIR)/system_init.o $(CFLAGS)

$(TARGET) : $(OBJS)
	$(LD) -Ttext 0x00000000 $(BUILD_DIR)/startup.o $(BUILD_DIR)/system_init.o $(wildcard $(OBJS_DIR)/*.o) -o $@ $(LIBS)
	$(OBJ_COPY) -O binary -S $@ $@.bin
	$(OBJ_DUMP) -D -m arm $@ > $@.dis

%.o : $(SRC_DIR)/%.c
	$(CC) -c $< -o $(OBJS_DIR)/$@ $(CFLAGS)

.PHONY : clean
	
clean :
	rm -rf $(OBJS_DIR) $(TARGET)* *.o
