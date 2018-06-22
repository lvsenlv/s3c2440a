@CLOCK_FREQ:
@   1: FCLK=12MHZ,      HCLK=12MHZ,     PCLK=12MHZ
@   2: FCLK=400MHZ,     HCLK=100MHZ,    PCLK=50MHZ
@   3: FCLK=400MHZ,     HCLK=100MHZ,    PCLK=100MHZ
@   4: FCLK=400MHZ,     HCLK=133HZ,     PCLK=133HZ

.equ CLOCK_FREQ, 4

.text
.global _start

_start :
    b   Reset
@定义中断向量
@@0x04: 未定义指令中止模式的向量地址
handle_undef:
    b   handle_undef

@@0x08: 管理模式的向量地址，通过SWI指令进入此模式
handle_SWI:
    b   handle_SWI

@@0x0c: 指令预取终止导致的异常的向量地址
handle_prefetch_abort:
    b   handle_prefetch_abort

@@0x10: 数据访问终止导致的异常的向量地址
handle_data_abort:
    b   handle_data_abort

@@0x14: 保留
handle_not_used:
    b   handle_not_used

@@0x18: 中断模式的向量地址
    b   handle_IRQ

@@0x1c: 快中断模式的向量地址
handle_FIQ:
    b   handle_FIQ

@CPSR:
@  bit7: 1 - Diable FIQ    0 - Enable FIQ
@  bit6: 1 - Diable IRQ    0 - Enable IRQ
@  bit5: Status
@  bit0~bit4: 
@        10000 - 用户模式
@        10001 - FIQ模式
@        10010 - IRQ模式
@        10011 - 管理模式
@        10111 - 中止模式
@        11111 - 系统模式

Reset:
    @Close watch dog
    mov     r0, #0x53000000
    ldr     r1, [r0]
    bic     r1, r1, #0x20
    str     r1, [r0]

    @Initialize clock
    mov     r0, #0x4C000000

.if CLOCK_FREQ==2
    @@Config CLKDIVN, HCLK=FCLK/4, PCLK=HCLK/2
    ldr     r1, [r0, #0x14]
    bic     r1, r1, #0x7
    orr     r1, r1, #0x5
    str     r1, [r0, #0x14]
    ldr     r1, [r0, #0x18]
    bic     r1, r1, #0x200
    str     r1, [r0, #0x18]
.elseif CLOCK_FREQ==3
    @@Config CLKDIVN, HCLK=FCLK/4, PCLK=HCLK
    ldr     r1, [r0, #0x14]
    bic     r1, r1, #0x7
    orr     r1, r1, #0x4
    str     r1, [r0, #0x14]
    ldr     r1, [r0, #0x18]
    bic     r1, r1, #0x200
    str     r1, [r0, #0x18]
.elseif CLOCK_FREQ==4
    @@Config CLKDIVN, HCLK=FCLK/3, PCLK=HCLK
    ldr     r1, [r0, #0x14]
    bic     r1, r1, #0x7
    orr     r1, r1, #0x6
    str     r1, [r0, #0x14]
    ldr     r1, [r0, #0x18]
    bic     r1, r1, #0x100
    str     r1, [r0, #0x18]
.endif

.if (CLOCK_FREQ > 1)
    @Set as asynchronous bus modes
    mrc     p15, 0, r1, c1, c0, 0
    orr     r1, r1, #0xC0000000
    mcr     p15, 0, r1, c1, c0, 0
    
    @@Config MPLLCON, FCLK=400MHZ
    ldr     r1, =0x5C011
    str     r1, [r0, #0x4]
.endif

    @Initialize SDRAM
    mov     r0, #0x48000000
    @@Config BWSCON
    ldr     r1, [r0]
    bic     r1, r1, #0xF000000
    orr     r1, r1, #0x2000000
    str     r1, [r0]
    @@Config BANKCON6
    ldr     r1, [r0, #0x1C]
    bic     r1, r1, #0x18000
    bic     r1, r1, #0xF
    orr     r1, r1, #0x18000
    orr     r1, r1, #0x5
    str     r1, [r0, #0x1C]
    @@Config REFRESH
.if CLOCK_FREQ==1
    ldr     r1, =0x9C07A3 @HCLK=12MHZ
.elseif CLOCK_FREQ==2
    ldr     r1, =0x9C04F4 @HCLK=100MHZ
.elseif CLOCK_FREQ==3
    ldr     r1, =0x9C04F4 @HCLK=100MHZ
.elseif CLOCK_FREQ==4
    ldr     r1, =0x9C03F2 @HCLK=133MHZ
.endif
    str     r1, [r0, #0x24]
    @@Config BANKSIZE 
    ldr     r1, =0xB1
    str     r1, [r0, #0x28]
    @@Config MRSRB6
    ldr     r1, =0x30
    str     r1, [r0, #0x2C]

.if 0
    @Copy SRAM to SDRAM
    mov     r0, #1024
    ldr     r1, =0x30000000
    ldr     r2, =0x1000
sram_to_sdram:
    ldr     r3, [r0], #0x4
    str     r3, [r1], #0x4
    cmp     r0, r2
    bne     sram_to_sdram
.endif

    @Initialize LED GPIOF 4/5/6
    mov     r0, #0x56000000
    ldr     r1, [r0, #0x50]
    bic     r1, r1, #0x3F00
    orr     r1, r1, #0x1500
    str     r1, [r0, #0x50]
    ldr     r1, [r0, #0x54]
    orr     r1, r1, #0x70
    str     r1, [r0, #0x54]
    
    ldr     sp, =4096
    bl      sys_init

    msr     cpsr_c, #0xd2           @ 进入中断模式
    ldr     sp, =0x34000000         @ 设置中断模式栈指针
    
    msr     cpsr_c, #0xd3           @ 进入管理模式，上电后默认进入管理模式
    ldr     sp, =0x33F00000         @ 设置用户模式栈指针
    ldr     pc, =IRQ_init           @ 初始化中断配置
    msr     cpsr_c, #0x53           @ 开IRQ中断
    ldr     lr, =halt_loop          @ 设置返回地址
    ldr     pc, =main

halt_loop :
    b       halt_loop

handle_IRQ:
    sub     lr, lr, #4              @ 计算返回地址
    stmdb   sp!, {r0-r12, lr}       @ 保存使用到的寄存器，注意：此时的sp是中断模式的sp，初始值是上面设置的0x34000000
    ldr     lr, =IRQ_exit           @ 设置调用ISR即EINT_Handle函数后的返回地址  
    ldr     pc, =IRQ_process        @ 调用中断服务函数，在interrupt.c中
    
IRQ_exit:
    ldmia   sp!, {r0-r12, pc}^      @ 中断返回，^表示将spsr的值复制到cpsr