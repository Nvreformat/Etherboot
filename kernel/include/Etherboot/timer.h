#ifndef TIMER_H
#define TIMER_H

#include <Etherboot/interrupt.h>
#include <Etherboot/sysconfig.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CLOCKHZ	1000000
#define HZ 100	
#define MSEC2HZ(msec) ((msec) * HZ / 1000)

typedef void KernelTimerHandler(unsigned Timer, void* Param, void* Context);

typedef struct TKernelTimer
{
	KernelTimerHandler* Handler;
	unsigned ElapsesAt;
	void* Param;
	void* Context;
} KernelTimer;

void SetupTimer();

void TimerTuneMsDelay();
void TimerInterruptHandler(void* Param);

uint64_t TimerGetClockTicks();
unsigned TimerGetTicks();
unsigned TimerGetTime();

unsigned TimerStartKernelTimer(unsigned Delay, KernelTimerHandler* Handler, void* Param, void* Context);
void TimerCancelKernelTimer(unsigned Timer);

void TimerMsDelay(unsigned MilliSeconds);
void TimerusDelay(unsigned MicroSeconds);
void TimerSimpleMsDelay (unsigned MilliSeconds);
void TimerSimpleusDelay (unsigned MicroSeconds);
uint64_t GetTimestamp();

#ifdef __cplusplus
}
#endif

#endif
