#ifndef INTERRUPT_H
#define INTERRUPT_H

#include <Etherboot/bcm2835.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ARM_EXCEPTION_TABLE_BASE	0x00000000
#define ARM_OPCODE_BRANCH(distance)	(0xEA000000 | (distance))
#define ARM_DISTANCE(from, to)		((u32 *) &(to) - (u32 *) &(from) - 2)

#define ARM_IC_IRQ_PENDING(irq)	(  (irq) < ARM_IRQ2_BASE	\
				 ? ARM_IC_IRQ_PENDING_1		\
				 : ((irq) < ARM_IRQBASIC_BASE	\
				   ? ARM_IC_IRQ_PENDING_2	\
				   : ARM_IC_IRQ_BASIC_PENDING))
#define ARM_IC_IRQS_ENABLE(irq)	(  (irq) < ARM_IRQ2_BASE	\
				 ? ARM_IC_ENABLE_IRQS_1		\
				 : ((irq) < ARM_IRQBASIC_BASE	\
				   ? ARM_IC_ENABLE_IRQS_2	\
				   : ARM_IC_ENABLE_BASIC_IRQS))
#define ARM_IC_IRQS_DISABLE(irq) (  (irq) < ARM_IRQ2_BASE	\
				 ? ARM_IC_DISABLE_IRQS_1	\
				 : ((irq) < ARM_IRQBASIC_BASE	\
				   ? ARM_IC_DISABLE_IRQS_2	\
				   : ARM_IC_DISABLE_BASIC_IRQS))
#define ARM_IRQ_MASK(irq)	(1 << ((irq) & (ARM_IRQS_PER_REG-1)))

typedef struct
{
	u32 Reset;
	u32 UndefinedInstruction;
	u32 SupervisorCall;
	u32 PrefetchAbort;
	u32 DataAbort;
	u32 Unused;
	u32 IRQ;
	u32 FIQ;
} ExceptionTable;

typedef void IRQHandler(void* Param);

void SetupInterrupts();
void InterruptSystemConnectIRQ(unsigned IRQ, IRQHandler* Handler, void* Param);
void InterruptSystemDisconnectIRQ(unsigned IRQ);
void InterruptSystemEnableIRQ(unsigned IRQ);
void InterruptSystemDisableIRQ(unsigned IRQ);
int InterruptSystemCallIRQHandler(unsigned IRQ);

void InterruptHandler(void);
void IRQStub(void);

#ifdef __cplusplus
}
#endif

#endif
