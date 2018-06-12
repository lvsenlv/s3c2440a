TARGET_BASE_NAME    = libc.a

CC                  = arm-linux-gcc
LD                  = arm-linux-ld
AR                  = arm-linux-ar
OBJCOPY             = arm-linux-objcopy
OBJDUMP             = arm-linux-objdump

SRC_DIR             = ../src
OBJ_DIR             = ./object
INC_DIR 			= ../include
LIB_DIR             = ./

OBJS                = div64.o lib1funcs.o ctype.o muldi3.o printf.o string.o vsprintf.o
LIBS                = 
DEF                 = 
CFLAGS              = -Wall -O3 -nostdinc \
					  -I$(INC_DIR) \
					  -L$(LIB_DIR)
TARGET              = ../../$(TARGET_BASE_NAME)

$(shell mkdir -p $(OBJ_DIR))

all : $(TARGET)

ifeq ($(wildcard $(TARGET)),)
$(TARGET): $(OBJS)
	${AR} -r -o $@ $(wildcard $(OBJ_DIR)/*.o)
endif
	
%.o : $(SRC_DIR)/%.c
	$(CC) -c $<  -o $(OBJ_DIR)/$@ $(DEF) $(CFLAGS)

%.o : $(SRC_DIR)/%.S
	$(CC) -c $<  -o $(OBJ_DIR)/$@ $(DEF) $(CFLAGS)

clean:
	rm -rf $(OBJ_DIR) $(TARGET)
	
