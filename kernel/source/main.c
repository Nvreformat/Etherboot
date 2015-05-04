#include <USPI/uspi.h>
#include <Etherboot/string.h>
#include <Etherboot/memutil.h>
#include <Etherboot/memory.h>
#include <Etherboot/propertytags.h>
#include <Etherboot/interrupt.h>
#include <Etherboot/sysutils.h>
#include <Etherboot/ethernet.h>
#include <../../etherbootdefs.h>

typedef void (*KernelFunction)();

uint32_t ReceivedBytes;
uint32_t KernelSize;

KernelFunction Kernel = (KernelFunction) KERNEL_ADDRESS;
extern EthbootPacket* SndPacket;

void BootNewKernel()
{
	InterruptSystemDisconnectIRQ(3);
	InterruptSystemDisconnectIRQ(9);
	DisableInterrupts();
	
	PropertyTagPowerState PowerState;
	PowerState.DeviceId = 3;
	PowerState.State = POWER_STATE_OFF;
	PropertyTagsGetTag(PROPTAG_SET_POWER_STATE, &PowerState, sizeof(PowerState), 8);
	
	DisposeMemory();

	Kernel();
}

void ProcessPacket(EthbootPacket* RecvPacket)
{
	if (RecvPacket->Code == CODE_ESTABLISH_CONNECTION)
	{
		LogRemote("Logging started");
		LogRemote("Prepared to receive kernel");
			
		SndPacket->Code = CODE_CONNECTION_ESTABLISHED;
		SndPacket->DataLength = 0;
		SendPacket(SndPacket);
	}
	else if (RecvPacket->Code == CODE_SEND_KERNEL_SIZE)
	{
		KernelSize = *((uint32_t*)RecvPacket->Data);
		
		LogRemote(StringConcat("Received kernel size, it is ", uitoa(KernelSize)));
				
		SndPacket->Code = CODE_GET_KERNEL;
		SndPacket->DataLength = 0;
		SendPacket(SndPacket);
	}
	else if (RecvPacket->Code == CODE_SEND_KERNEL_PART)
	{
		MemoryCopy(RecvPacket->Data, (void*)(KERNEL_ADDRESS + ReceivedBytes), RecvPacket->DataLength);
		ReceivedBytes += RecvPacket->DataLength;
			
		if (ReceivedBytes < KernelSize)
		{
			SndPacket->Code = CODE_GET_KERNEL;
			SndPacket->DataLength = 0;
			SendPacket(SndPacket);
		}
		else
		{
			LogRemote("Booting...");
					
			SndPacket->Code = CODE_BOOTING;
			SndPacket->DataLength = 0;
			SendPacket(SndPacket);
			
			BootNewKernel();
		}
	}
}

void KernelMain(void)
{
	ReceivedBytes = 0;

	while (1)
	{
		EthbootPacket* RecvPacket = ReceivePacket();
		
		if (RecvPacket)
			ProcessPacket(RecvPacket);
	}
}
