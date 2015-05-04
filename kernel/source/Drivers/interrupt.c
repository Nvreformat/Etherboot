#include <Etherboot/interrupt.h>
#include <Etherboot/sysutils.h>
#include <Etherboot/bcm2835.h>

IRQHandler* IRQHandlers[IRQ_LINES];
void* IRQParams[IRQ_LINES];

void SetupInterrupts()
{
	for (unsigned IRQ = 0; IRQ < IRQ_LINES; IRQ++)
	{
		IRQHandlers[IRQ] = 0;
		IRQParams[IRQ] = 0;
	}
	
	ExceptionTable* Table = (ExceptionTable*) ARM_EXCEPTION_TABLE_BASE;
	Table->IRQ = ARM_OPCODE_BRANCH(ARM_DISTANCE(Table->IRQ, IRQStub));

	CleanDataCache();
	DataSyncBarrier();

	InvalidateInstructionCache();
	FlushBranchTargetCache();
	DataSyncBarrier();

	InstructionSyncBarrier();

	DataMemBarrier();

	Write32(ARM_IC_FIQ_CONTROL, 0);

	Write32(ARM_IC_DISABLE_IRQS_1, (u32) -1);
	Write32(ARM_IC_DISABLE_IRQS_2, (u32) -1);
	Write32(ARM_IC_DISABLE_BASIC_IRQS, (u32) -1);

	// Ack pending IRQs
	Write32(ARM_IC_IRQ_BASIC_PENDING, Read32(ARM_IC_IRQ_BASIC_PENDING));
	Write32(ARM_IC_IRQ_PENDING_1, Read32(ARM_IC_IRQ_PENDING_1));
	Write32(ARM_IC_IRQ_PENDING_2, Read32(ARM_IC_IRQ_PENDING_2));

	DataMemBarrier();

	EnableInterrupts();
}

void InterruptSystemConnectIRQ(unsigned IRQ, IRQHandler* Handler, void* Param)
{
	IRQHandlers[IRQ] = Handler;
	IRQParams[IRQ] = Param;

	InterruptSystemEnableIRQ(IRQ);
}

void InterruptSystemDisconnectIRQ(unsigned IRQ)
{
	InterruptSystemDisableIRQ(IRQ);

	IRQHandlers[IRQ] = 0;
	IRQParams[IRQ] = 0;
}

void InterruptSystemEnableIRQ(unsigned IRQ)
{
	DataMemBarrier();

	Write32(ARM_IC_IRQS_ENABLE(IRQ), ARM_IRQ_MASK(IRQ));

	DataMemBarrier();
}

void InterruptSystemDisableIRQ(unsigned IRQ)
{
	DataMemBarrier();

	Write32(ARM_IC_IRQS_DISABLE(IRQ), ARM_IRQ_MASK(IRQ));

	DataMemBarrier();
}

int InterruptSystemCallIRQHandler(unsigned IRQ)
{
	IRQHandler* Handler = IRQHandlers[IRQ];

	if (Handler != 0)
	{
		(*Handler)(IRQParams[IRQ]);
		
		return 1;
	}
	else
	{
		InterruptSystemDisableIRQ(IRQ);
	}
	
	return 0;
}

void InterruptHandler(void)
{
	DataMemBarrier();
	
	for (unsigned IRQ = 0; IRQ < IRQ_LINES; IRQ++)
	{
		u32 PendReg = ARM_IC_IRQ_PENDING(IRQ);
		u32 IRQMask = ARM_IRQ_MASK(IRQ);
		
		if (Read32(PendReg) & IRQMask)
		{
			if (InterruptSystemCallIRQHandler(IRQ))
			{
				Write32(PendReg, IRQMask);
			
				DataMemBarrier();

				return;
			}

			Write32(PendReg, IRQMask);
		}
	}

	DataMemBarrier();
}
