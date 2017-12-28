TARGET = s3c2440a
ROOT_DIR = $(shell cd ../ && pwd)
INCS = -I$(ROOT_DIR)/include
BUILD_DIR = $(ROOT_DIR)/build
OBJS_DIR = $(ROOT_DIR)/build/object
SRC_DIR = $(ROOT_DIR)/src

OBJS = led.o uart.o main.o

CC := arm-linux-gcc
OBJ_DUMP := arm-linux-objdump
OBJ_COPY := arm-linux-objcopy
LD := arm-linux-ld
CFLAGS := $(INCS) -Wall -Wno-strict-aliasing -O0
LIBS :=
$(shell mkdir -p $(OBJS_DIR))

all : before $(TARGET)

before :
	$(CC) -c $(SRC_DIR)/startup.s -o $(BUILD_DIR)/startup.o
	$(CC) -c $(SRC_DIR)/system_init.c -o $(BUILD_DIR)/system_init.o $(CFLAGS)

$(TARGET) : $(OBJS)
	$(LD) -Tlds $(wildcard $(OBJS_DIR)/*.o) -o $@
	$(OBJ_COPY) -O binary -S $@ $@.bin
	$(OBJ_DUMP) -D -m arm $@ > $@.dis

%.o : $(SRC_DIR)/%.c
	$(CC) -c $< -o $(OBJS_DIR)/$@ $(CFLAGS) $(LIBS)

.PHONY : clean
	
clean :
	rm -rf $(OBJS_DIR) $(TARGET)* *.o
