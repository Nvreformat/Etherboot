#include <Etherboot/timer.h>
#include <Etherboot/sysutils.h>
#include <Etherboot/bcm2835.h>
#include <Etherboot/sysconfig.h>
#include <Etherboot/alloc.h>

volatile unsigned Ticks;
volatile unsigned Time;
volatile KernelTimer Timers[KERNEL_TIMERS];
unsigned _MsDelay;
unsigned _UsDelay;

void DelayLoop(int Cycles);

void SetupTimer()
{
	Ticks = 0;
	Time = 0;
#ifdef ARM_DISABLE_MMU
	_MsDelay = 12500;
#else
	_MsDelay = 350000;
#endif
	_UsDelay = _MsDelay / 1000;

	for (unsigned Timer = 0; Timer < KERNEL_TIMERS; Timer++)
	{
		Timers[Timer].Handler = 0;
	}
	
	InterruptSystemConnectIRQ(ARM_IRQ_TIMER3, TimerInterruptHandler, 0);

	DataMemBarrier();

	Write32(ARM_SYSTIMER_CLO, -(30 * CLOCKHZ));
	Write32(ARM_SYSTIMER_C3, Read32(ARM_SYSTIMER_CLO) + CLOCKHZ / HZ);
	TimerTuneMsDelay();

	DataMemBarrier();
}

uint64_t TimerGetClockTicks()
{
	volatile uint64_t* TimestampAddr = (uint64_t*)0x20003004;

	DataMemBarrier();

	uint64_t Result = *TimestampAddr;

	DataMemBarrier();

	return Result;
}

unsigned TimerGetTicks()
{
	return Ticks;
}

unsigned TimerGetTime()
{
	return Time;
}

unsigned TimerStartKernelTimer(unsigned Delay, KernelTimerHandler* Handler, void* Param, void* Context)
{
	EnterCritical();

	unsigned Timer;
	for (Timer = 0; Timer < KERNEL_TIMERS; Timer++)
	{
		if (Timers[Timer].Handler == 0)
		{
			break;
		}
	}

	if (Timer >= KERNEL_TIMERS)
	{
		LeaveCritical();

		return 0;
	}

	Timers[Timer].Handler = Handler;
	Timers[Timer].ElapsesAt = Ticks+Delay;
	Timers[Timer].Param = Param;
	Timers[Timer].Context = Context;

	LeaveCritical();

	return Timer+1;
}

void TimerCancelKernelTimer(unsigned Timer)
{
	Timers[Timer-1].Handler = 0;
}

void TimerMsDelay(unsigned MilliSeconds)
{
	if (MilliSeconds > 0)
	{
		unsigned Cycles =  _MsDelay * MilliSeconds;

		DelayLoop(Cycles);
	}
}

void TimerusDelay(unsigned MicroSeconds)
{
	if (MicroSeconds > 0)
	{
		unsigned Cycles =  _UsDelay * MicroSeconds;

		DelayLoop(Cycles);
	}
}

void TimerSimpleMsDelay(unsigned MilliSeconds)
{
	if (MilliSeconds > 0)
	{
		TimerSimpleusDelay(MilliSeconds * 1000);
	}
}

void TimerSimpleusDelay(unsigned MicroSeconds)
{
	if (MicroSeconds > 0)
	{
		unsigned Ticks = MicroSeconds * (CLOCKHZ / 1000000);

		DataMemBarrier();

		unsigned StartTicks = Read32(ARM_SYSTIMER_CLO);
		while (Read32(ARM_SYSTIMER_CLO) - StartTicks < Ticks);

		DataMemBarrier();
	}
}

void TimerPollKernelTimers()
{
	EnterCritical();

	for (unsigned TimerIndex = 0; TimerIndex < KERNEL_TIMERS; TimerIndex++)
	{
		volatile KernelTimer *Timer = &Timers[TimerIndex];
		KernelTimerHandler *Handler = Timer->Handler;
		
		if (Handler != 0)
		{
			if ((int) (Timer->ElapsesAt - Ticks) <= 0)
			{
				Timer->Handler = 0;

				(*Handler)(TimerIndex+1, Timer->Param, Timer->Context);
			}
		}
	}

	LeaveCritical();
}

void TimerInterruptHandler(void* Param)
{
	DataMemBarrier();

	u32 Compare = Read32(ARM_SYSTIMER_C3) + CLOCKHZ / HZ;
	Write32(ARM_SYSTIMER_C3, Compare);
	
	if (Compare < Read32(ARM_SYSTIMER_CLO))
	{
		Compare = Read32(ARM_SYSTIMER_CLO) + CLOCKHZ / HZ;
		Write32(ARM_SYSTIMER_C3, Compare);
	}

	Write32(ARM_SYSTIMER_CS, 1 << 3);

	DataMemBarrier();

	if (++Ticks % HZ == 0)
	{
		Time++;
	}

	TimerPollKernelTimers();
}

void TimerTuneMsDelay()
{
	unsigned Ticks = TimerGetTicks();
	TimerMsDelay(1000);
	Ticks = TimerGetTicks() - Ticks;

	unsigned Factor = 100 * HZ / Ticks;
	_MsDelay = _MsDelay * Factor / 100;
	_UsDelay = (_MsDelay + 500) / 1000;
}
