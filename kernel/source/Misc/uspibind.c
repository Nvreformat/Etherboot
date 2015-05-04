#include <stdarg.h>
#include <USPI/uspios.h>
#include <Etherboot/timer.h>
#include <Etherboot/interrupt.h>
#include <Etherboot/alloc.h>
#include <Etherboot/memutil.h>
#include <Etherboot/propertytags.h>

void* malloc(unsigned Size)
{
	return MemoryAllocate(Size);
}

void free(void* Block)
{
	MemoryFree(Block);
}

void MsDelay(unsigned MilliSeconds)
{
	TimerMsDelay(MilliSeconds);
}

void usDelay(unsigned MicroSeconds)
{
	TimerusDelay(MicroSeconds);
}

unsigned StartKernelTimer(unsigned Delay, TKernelTimerHandler *Handler, void *Param, void *Context)
{
	return TimerStartKernelTimer(Delay, Handler, Param, Context);
}

void CancelKernelTimer(unsigned Timer)
{
	TimerCancelKernelTimer(Timer);
}

void ConnectInterrupt(unsigned IRQ, TInterruptHandler *Handler, void *Param)
{
	InterruptSystemConnectIRQ(IRQ, Handler, Param);
}

int SetPowerStateOn(unsigned DeviceId)
{
	PropertyTagPowerState PowerState;
	PowerState.DeviceId = DeviceId;
	PowerState.State = POWER_STATE_ON | POWER_STATE_WAIT;
	if (!PropertyTagsGetTag(PROPTAG_SET_POWER_STATE, &PowerState, sizeof(PowerState), 8) || (PowerState.State & POWER_STATE_NO_DEVICE) || !(PowerState.State & POWER_STATE_ON))
	{
		return 0;
	}

	return 1;
}

int GetMACAddress(unsigned char Buffer[6])
{
	PropertyTagMACAddress MACAddress;
	if (!PropertyTagsGetTag(PROPTAG_GET_MAC_ADDRESS, &MACAddress, sizeof(MACAddress), 0))
	{
		return 0;
	}

	MemoryCopy(MACAddress.Address, Buffer, 6);

	return 1;
}

void LogWrite(const char* Source, unsigned Severity, const char* Message, ...)
{

}

void uspi_assertion_failed(const char* Expr, const char* File, unsigned Line)
{
	
}

void DebugHexdump(const void* Buffer, unsigned BufLen, const char* Source)
{
	
}
