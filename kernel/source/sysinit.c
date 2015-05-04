#include <USPI/uspi.h>
#include <Etherboot/memory.h>
#include <Etherboot/interrupt.h>
#include <Etherboot/timer.h>
#include <Etherboot/sysutils.h>
#include <Etherboot/sysconfig.h>
#include <Etherboot/memutil.h>

void KernelMain();

void SystemInit(void)
{
	#if RASPPI != 1
	InvalidateDataCache ();
	#endif

	extern unsigned char __BSS_START;
	extern unsigned char __BSS_END;
	for(unsigned char *BSS = &__BSS_START; BSS < &__BSS_END; BSS++)
	{
		*BSS = 0;
	}
	
	SetupMemory();
	SetupInterrupts();
	SetupTimer();
	USPiInitialize();

	KernelMain();
}
