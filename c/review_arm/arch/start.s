.section .text @area ,code
.global _start

_start:
	@STEP 1:��ʼ���쳣������
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
	@STEP 2:��ʼ������CPU��Ӳ������CACHE....
	ldr r0,=0x20008000
	mcr p15,0,r0,c12,c0,0 @����ARM�쳣������ַΪ0x20008000

	@STEP 3:��ʼ����ģʽ��ջ
	@��ʼ��SVCģʽ��ջ
	ldr r0,=0x30000000
	mov sp,r0

	@��ʼ��IRQģʽ��ջ
	mrs r1,cpsr
	bic r1,r1,#0x1f
	orr r1,r1,#0x12
	msr cpsr_c,r1
	sub r0,r0,#0x1000
	mov sp,r0    @IRQģʽ��sp
	@��IRQ�ж�
	mrs r1,cpsr
	bic r1,r1,#0x80
	msr cpsr_c,r1

	@�л���USERģʽ
	mrs r1,cpsr
	bic r1,r1,#0x1f
	orr r1,r1,#0x10
	msr cpsr_c,r1

	@��ʼ��USERģʽ��ջ
	sub r0,r0,#0x1000
	mov sp,r0

	@STEP 4 :��ʼ��ϵͳӲ����ϵͳʱ�� MEM

	@STEP 5: ���BSS��

clear_bss:
	ldr r0, =__start_bss @ע��map.lds�ű�
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
