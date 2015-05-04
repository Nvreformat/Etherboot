#ifndef SYSUTILS_H
#define SYSUTILS_H

#define	EnableInterrupts()	__asm volatile ("cpsie i")
#define	DisableInterrupts()	__asm volatile ("cpsid i")


#if RASPPI == 1

#define InvalidateInstructionCache()	\
				__asm volatile ("mcr p15, 0, %0, c7, c5,  0" : : "r" (0) : "memory")
#define FlushPrefetchBuffer()	__asm volatile ("mcr p15, 0, %0, c7, c5,  4" : : "r" (0) : "memory")
#define FlushBranchTargetCache()	\
				__asm volatile ("mcr p15, 0, %0, c7, c5,  6" : : "r" (0) : "memory")
#define InvalidateDataCache()	__asm volatile ("mcr p15, 0, %0, c7, c6,  0" : : "r" (0) : "memory")
#define CleanDataCache()	__asm volatile ("mcr p15, 0, %0, c7, c10, 0" : : "r" (0) : "memory")

#define DataSyncBarrier()	__asm volatile ("mcr p15, 0, %0, c7, c10, 4" : : "r" (0) : "memory")
#define DataMemBarrier() 	__asm volatile ("mcr p15, 0, %0, c7, c10, 5" : : "r" (0) : "memory")

#define InstructionSyncBarrier() FlushPrefetchBuffer()
#define InstructionMemBarrier()	FlushPrefetchBuffer()

#else

#define InvalidateInstructionCache()	\
				__asm volatile ("mcr p15, 0, %0, c7, c5,  0" : : "r" (0) : "memory")
#define FlushPrefetchBuffer()	__asm volatile ("isb" ::: "memory")
#define FlushBranchTargetCache()	\
				__asm volatile ("mcr p15, 0, %0, c7, c5,  6" : : "r" (0) : "memory")

void InvalidateDataCache (void) __attribute__ ((optimize (3)));
void CleanDataCache (void) __attribute__ ((optimize (3)));

void InvalidateDataCacheRange (u32 nAddress, u32 nLength) __attribute__ ((optimize (3)));
void CleanDataCacheRange (u32 nAddress, u32 nLength) __attribute__ ((optimize (3)));
void CleanAndInvalidateDataCacheRange (u32 nAddress, u32 nLength) __attribute__ ((optimize (3)));

#define DataSyncBarrier()	__asm volatile ("dsb" ::: "memory")
#define DataMemBarrier() 	__asm volatile ("dmb" ::: "memory")

#define InstructionSyncBarrier() __asm volatile ("isb" ::: "memory")
#define InstructionMemBarrier()	__asm volatile ("isb" ::: "memory")

#endif

#define CompilerBarrier()	__asm volatile ("" ::: "memory")

#ifdef __cplusplus
extern "C" {
#endif

void EnterCritical (void);
void LeaveCritical (void);
void Halt(void);
void Reboot(void);

u32 Read32(u32 Address);
void Write32(u32 Address, u32 Value);

#ifdef __cplusplus
}
#endif

#endif
