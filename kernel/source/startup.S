#include <Etherboot/sysconfig.h>

.section .init

.globl _start
_start:
	cps	#0x1F
	ldr	sp, =MEM_KERNEL_STACK
	b	SystemInit

.section .text

.globl Write32
Write32:
	str r1,[r0]
	bx lr

.globl Read32
Read32:
	ldr r0,[r0]
	bx lr
	
.globl DelayLoop
DelayLoop:
	subs	r0, r0, #1
	bhi	DelayLoop
	mov	pc, lr

.globl IRQStub
IRQStub:
	ldr	sp, =MEM_IRQ_STACK
	sub	lr, lr, #4
	stmfd	sp!, {r0-r12, lr}
	bl	InterruptHandler
	ldmfd	sp!, {r0-r12, pc}^
