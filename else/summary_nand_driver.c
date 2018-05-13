读取页操作：
1. 写地址问题：
    对于型号为 K9F2G08U0C-S 的 nand flash 而言，Page Size : (2K + 64)Byte
    对于 s3c2440a 而言，nand flash 的地址为 0x00000000~0x0FFFFFFF，256MiB
    对于某个地址，其 column 地址为 bit0~bit10，其 row/page 地址为 bit11~bit27
    对于读取页操作而言，只需要 row/page 地址即可，写地址代码如下所示：
    
    NAND->NFADDR = 0;                               //A0 ~ A7
    NAND->NFADDR = 0;                               //A8 ~ A11
    NAND->NFADDR = (source_addr >> 11) & 0xFF;      //A12 ~ A19
    NAND->NFADDR = (source_addr >> 19) & 0xFF;      //A20 ~ A27
    NAND->NFADDR = (source_addr >> 27) & 0x01;      //A28

    注意：
    
    uint8_t *ptr = (uint8_t *)&NAND->NFADDR;
    *ptr = 0;
    *ptr = 0;
    *ptr = (source_addr >> 11) & 0xFF;
    *ptr = (source_addr >> 19) & 0xFF;
    *ptr = (source_addr >> 27) & 0xFF;

    上述代码存在风险，若开启了编译优化选项，上述5行代码会被优化为1条，
    因为编译器认为连续对 ptr 做了5次赋值，前4次的结果会被第5次覆盖，认为前4次操作无意义，
    并将前4步省略，最终只编译 “ *ptr = (source_addr >> 27) & 0xFF; ” 此行代码，因此会造成错误。
    而 NAND->NFADDR 此变量，声明为了 volatile 类型，以此避免编译器进行优化。
    所以要使用上述代码，则改为 volatile uint8_t *ptr = (volatile uint8_t *)&NAND->NFADDR;

2. 读取数据寄存器的问题
    注意如下两块代码的差异：

    (1):
    for(j = 0; j < NF_PAGE_SIZE; j++)
    {
        *target_ptr++ = NAND->NFDATA;
    }

    (2):
    for(j = 0; j < NF_PAGE_SIZE; j++)
    {
        *target_ptr++ = *(uint8_t *)&NAND->NFDATA;
    }

    其中:
    char *target_ptr;       //函数传入的参数
    __IO uint32_t NFDATA;   //结构体成员
    

    (1)的汇编代码
    mov	r3, #1308622848	; 0x4e000000
    ldr	r2, [r3, #16]
    strb	r2, [ip], #1

    (2)的汇编代码
    mov r3, #1308622848 ; 0x4e000000
    ldrb    r2, [r3, #16]
    strb    r2, [ip], #1

    根据 K9F2G08U0C-S 手册 5.1 Page Read 所描述：
    Once the data in a page is loaded into the data registers, 
    they may be read out in 25ns cycle time by sequentially pulsing RE. 
    The repetitive high to low transitions of the RE clock make 
    the device output the data starting from the selected column address up to the last column address.
    也就是说，RE引脚不断地输出高低电平，nand设备会依次将数据输出到数据寄存器

    (1)中，从 0x4e000010 取出值，存放至 r2 寄存器，使用的是 ldr 指令
    (2)中，从 0x4e000010 取出值，存放至 r2 寄存器，使用的是 ldrb 指令

    对于 32bit 的 s3c2440a 芯片而言，ldr 指令是将 32bit 数据写入 r2 寄存器，
    而 ldrb 指令是将 8bit 数据写入 r2 寄存器
    无论(1)还是(2)，紧接着都是使用 strb 指令将 r2 写入目标地址

    对于(1)而言，使用 ldr 指令，已经将数据寄存器的 4 字节数据存放进了 r2，
    而后使用 strb 指令，则意味着丢掉其余数据，只将低 8 位数据写入目标地址，
    这样，就造成了数据丢失

    而对于(2)而言，使用 ldrb 指令，则只从数据寄存器取出 1 字节数据，并放入 r2，
    而后使用 strb 指令，将此 1 字节数据写入目标地址，并不会造成数据丢失

    所以(1)的写法是错误的
    另外，由于 NAND->NFDATA 已经声明为 volatile 类型，故(2)无需优化，若使用了非 volatile 类型，
    为防止编译优化，(2)可以修改为如下内容

    for(j = 0; j < NF_PAGE_SIZE; j++)
    {
        *target_ptr++ = *(volatile uint8_t *)data_ptr;
    }

3. 发送命令的问题
    注意如下两块代码的差异：

    (1):
    *(uint8_t *)&NAND->NFCMMD = 0x30;
    NF_WAIT_IDLE();

    (2):
    NF_WRITE_CMD(0x30);
    NF_WAIT_IDLE();

    其中：
    #define NF_WAIT_IDLE() \
            do \
            { \
                while(!(NAND->NFSTAT & 0x1)) \
                { \
                    delay(10); \
                } \
            }while(0)

    #define NF_WRITE_CMD(cmd) \
            do \
            { \
                NAND->NFCMMD = (uint8_t)cmd; \
            }while(0)

   上述代码可简化为：
    (2):
    *(uint8_t *)&NAND->NFCMMD = 0x30;
    while(!(NAND->NFSTAT & 0x1))
    {
        delay(10);
    }

    (2):
    NAND->NFCMMD = (uint8_t)0x30;
    while(!(NAND->NFSTAT & 0x1))
    {
        delay(10);
    }

    (1)的反汇编
    198:   e3a0144e    mov r1, #1308622848 ; 0x4e000000
    19c:   e5913020    ldr r3, [r1, #32]
    1a0:   e3a02030    mov r2, #48 ; 0x30
    1a4:   e3130001    tst r3, #1  ; 0x1
    1a8:   e5c12008    strb    r2, [r1, #8]
    1ac:   1a000006    bne 1cc <sys_nand_read+0x104>
    1b0:   e1a02001    mov r2, r1
    1b4:   e3a0300a    mov r3, #10 ; 0xa
    1b8:   e2533001    subs    r3, r3, #1  ; 0x1
    1bc:   1afffffd    bne 1b8 <sys_nand_read+0xf0>
    1c0:   e5921020    ldr r1, [r2, #32]
    1c4:   e3110001    tst r1, #1  ; 0x1
    1c8:   0afffff9    beq 1b4 <sys_nand_read+0xec>

    (2)的反汇编
    198:   e3a0144e    mov r1, #1308622848 ; 0x4e000000
    19c:   e3a03030    mov r3, #48 ; 0x30
    1a0:   e5813008    str r3, [r1, #8]
    1a4:   e5912020    ldr r2, [r1, #32]
    1a8:   e3120001    tst r2, #1  ; 0x1
    1ac:   1a000006    bne 1cc <sys_nand_read+0x104>
    1b0:   e1a02001    mov r2, r1
    1b4:   e3a0300a    mov r3, #10 ; 0xa
    1b8:   e2533001    subs    r3, r3, #1  ; 0x1
    1bc:   1afffffd    bne 1b8 <sys_nand_read+0xf0>
    1c0:   e5921020    ldr r1, [r2, #32]
    1c4:   e3110001    tst r1, #1  ; 0x1
    1c8:   0afffff9    beq 1b4 <sys_nand_read+0xec>

    留意：
    (1)
    198:   e3a0144e    mov r1, #1308622848 ; 0x4e000000
    19c:   e5913020    ldr r3, [r1, #32]
    1a0:   e3a02030    mov r2, #48 ; 0x30
    1a4:   e3130001    tst r3, #1  ; 0x1
    1a8:   e5c12008    strb    r2, [r1, #8]
    1ac:   1a000006    bne 1cc <sys_nand_read+0x104>

    (2)
    198:   e3a0144e    mov r1, #1308622848 ; 0x4e000000
    19c:   e3a03030    mov r3, #48 ; 0x30
    1a0:   e5813008    str r3, [r1, #8]
    1a4:   e5912020    ldr r2, [r1, #32]
    1a8:   e3120001    tst r2, #1  ; 0x1
    1ac:   1a000006    bne 1cc <sys_nand_read+0x104>

    对于(1)而言，
    19c: 将状态寄存器中的值写入 r3 寄存器
    1a0: 将 0x30 写入 r2 寄存器
    1a4: 判断 r3 寄存器中的值，其 bit1 是否被置位
    1a8：将 r2 的值写入命令寄存器
    1ac: 判断先前 tst 指令的结果，若被置位，则跳转到指定位置，即判断 nand 设备是否处于 busy 模式
    
    对于(2)而言，
    19c: 将 0x30 写入 r3 寄存器
    1a0: 将 r3 的值写入命令寄存器
    1a4: 将状态寄存器中的值写入 r2 寄存器
    1a8：判断 r2 寄存器中的值，其 bit1 是否被置位
    1ac: 判断 tst 指令的结果，若被置位，则跳转到指定位置，即判断 nand 设备是否处于 busy 模式

    对于(1)而言，先判断状态寄存器，后再将命令写入命令寄存器
    对于(2)而言，先将命令写入命令寄存器，再判断状态寄存器

    (1)的逻辑有误，未写入命令寄存器，则 nand 设备处于非 busy 模式，则导致直接跳出循环
    根据 K9F2G08U0C-S 手册 5.1 Page Read 所描述，写入命令后必须等待，直到 nand 设备处于非 busy 模式

    从C语言代码上，逻辑并未有问题，但编译结果，却将(1)的逻辑颠倒，导致错误
    由于编译优化导致逻辑错误，(1)应将代码修改为：
    
    *(volatile uint8_t *)&NAND->NFCMMD = 0x30;
    while(!(NAND->NFSTAT & 0x1))
    {
        delay(10);
    }

总结：
对于寄存器的操作，尤其要注意编译器优化导致的问题，
对于寄存器的操作，一定要使用 volatile 类型，让编译不要优化，严格的从寄存器读值并严格写入目标地址