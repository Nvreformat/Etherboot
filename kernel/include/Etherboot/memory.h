#ifndef MEMORY_H
#define MEMORY_H

#include <Etherboot/pagetable.h>

#if RASPPI == 1
#define MMU_MODE	(  ARM_CONTROL_MMU			\
			 | ARM_CONTROL_L1_CACHE			\
			 | ARM_CONTROL_L1_INSTRUCTION_CACHE	\
			 | ARM_CONTROL_BRANCH_PREDICTION	\
			 | ARM_CONTROL_EXTENDED_PAGE_TABLE)

#define TTBCR_SPLIT	3
#else
#define MMU_MODE	(  ARM_CONTROL_MMU			\
			 | ARM_CONTROL_L1_CACHE			\
			 | ARM_CONTROL_L1_INSTRUCTION_CACHE	\
			 | ARM_CONTROL_BRANCH_PREDICTION)

#define TTBCR_SPLIT	2
#endif

#ifdef __cplusplus
extern "C" {
#endif

void SetupMemory();
void DisposeMemory();
void MemorySystemEnableMMU();
u32 MemorySystemGetMemSize();

#ifdef __cplusplus
}
#endif

#endif
