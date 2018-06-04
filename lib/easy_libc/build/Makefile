TARGET_BASE_NAME    = libeasy_libc.a
CC                  = arm-linux-gcc
LD                  = arm-linux-ld
AR                  = arm-linux-ar
OBJCOPY             = arm-linux-objcopy
OBJDUMP             = arm-linux-objdump
LIBS                = 
CFLAGS              = -Wall -O3 -nostdinc -fno-builtin
SRC_DIR             = ../src
OBJ_DIR             = ./object
LIB_DIR             = ./
OBJS                = easy_libc.o
DEF                 = 
INC                 = -I../include 
TARGET              = $(TARGET_BASE_NAME)

$(shell mkdir -p $(OBJ_DIR))

all : $(TARGET)

$(TARGET): $(OBJS)
	${AR} -r -o $@ $(wildcard $(OBJ_DIR)/*.o)
	
%.o : $(SRC_DIR)/%.c
	$(CC) -c $<  -o $(OBJ_DIR)/$@ $(DEF) $(INC) $(CFLAGS)

%.o : $(SRC_DIR)/%.s
	$(CC) -c $<  -o $(OBJ_DIR)/$@ $(DEF) $(INC) $(CFLAGS)

clean:
	rm -rf $(OBJ_DIR) $(TARGET_BASE_NAME)* 
	
