.section .text @area ,code
.global _start

_start:
	@STEP 1:初始化异常向量表
	b reset_handler
	nop
	nop
	@b swi_handler
	nop
	nop
	nop
	b irq_handler
	nop

irq_handler:
	sub lr,lr,#4
	stmfd sp! ,{r0-r12,lr}
	bl asm_do_irq
	ldmfd sp!,{r0-r12,pc}^


reset_handler:
	@STEP 2:初始化管理CPU的硬件，如CACHE....
	ldr r0,=0x20008000
	mcr p15,0,r0,c12,c0,0 @设置ARM异常向量地址为0x20008000

	@STEP 3:初始化各模式堆栈
	@初始化SVC模式堆栈
	ldr r0,=0x30000000
	mov sp,r0

	@初始化IRQ模式堆栈
	mrs r1,cpsr
	bic r1,r1,#0x1f
	orr r1,r1,#0x12
	msr cpsr_c,r1
	sub r0,r0,#0x1000
	mov sp,r0    @IRQ模式的sp
	@开IRQ中断
	mrs r1,cpsr
	bic r1,r1,#0x80
	msr cpsr_c,r1

	@切换到USER模式
	mrs r1,cpsr
	bic r1,r1,#0x1f
	orr r1,r1,#0x10
	msr cpsr_c,r1

	@初始化USER模式堆栈
	sub r0,r0,#0x1000
	mov sp,r0

	@STEP 4 :初始化系统硬件，系统时钟 MEM

	@STEP 5: 清除BSS段

clear_bss:
	ldr r0, =__start_bss @注意map.lds脚本
	ldr r1,=__end_bss
	mov r2,#0
bss_loop:
	cmp r0,r1
	strne r2,[r0],#4
	bne bss_loop

	b main

stop:
	b stop
	.end
