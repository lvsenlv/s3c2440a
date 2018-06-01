@CLOCK_FREQ:
@   1: FCLK=12MHZ,      HCLK=12MHZ,     PCLK=12MHZ
@   2: FCLK=400MHZ,     HCLK=100MHZ,    PCLK=50MHZ
@   3: FCLK=400MHZ,     HCLK=100MHZ,    PCLK=100MHZ
@   4: FCLK=400MHZ,     HCLK=133HZ,     PCLK=133HZ

.equ CLOCK_FREQ, 2

.text
.global _start

_start :
    @Close watch dog
    mov     r0, #0x53000000
    ldr     r1, [r0]
    bic     r1, r1, #0x20
    str     r1, [r0]

.if (CLOCK_FREQ > 1)
    @Set as asynchronous bus modes
    mrc     p15, 0, r1, c1, c0, 0
    orr     r1, r1, #0xC0000000
    mcr     p15, 0, r1, c1, c0, 0
    
    @Initialize clock
    mov     r0, #0x4C000000
    @@Config MPLLCON, FCLK=400MHZ
    ldr     r1, =0x5C011
    str     r1, [r0, #0x4]

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
    nop
    ldr     r1, [r0, #0x14]
    bic     r1, r1, #0x7
    orr     r1, r1, #0x6
    str     r1, [r0, #0x14]
    ldr     r1, [r0, #0x18]
    bic     r1, r1, #0x100
    str     r1, [r0, #0x18]
.endif
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
    
    ldr     sp, =0x34000000
    ldr     pc, =main

halt_loop :
    b   halt_loop
    
