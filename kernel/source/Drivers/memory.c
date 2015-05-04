#include <Etherboot/memory.h>
#include <Etherboot/sysutils.h>
#include <Etherboot/mmu.h>
#include <Etherboot/propertytags.h>
#include <Etherboot/alloc.h>
#include <Etherboot/sysconfig.h>

u32 MemorySize;
TPageTable *PageTable0;
TPageTable *PageTable1;

void SetupMemory()
{
	MemorySize = 0;
	PageTable0 = 0;
	PageTable1 = 0;

	PropertyTagMemory TagMemory;
	if (!PropertyTagsGetTag(PROPTAG_GET_ARM_MEMORY, &TagMemory, sizeof(TagMemory), 0))
	{
		TagMemory.BaseAddress = 0;
		TagMemory.Size = ARM_MEM_SIZE;
	}

	MemorySize = TagMemory.Size;

	MemoryInitialize(TagMemory.BaseAddress, MemorySize);

	PageTable0 = (TPageTable*) MemoryAllocate(sizeof(TPageTable));
	PageTable2(PageTable0, MemorySize);

	PageTable1 = (TPageTable*) MemoryAllocate(sizeof(TPageTable));
	PageTable(PageTable1);

	MemorySystemEnableMMU();
}

void DisposeMemory()
{
	// disable MMU
	u32 Control;
	asm volatile("mrc p15, 0, %0, c1, c0,  0" : "=r" (Control));
	Control &=  ~MMU_MODE;
	asm volatile("mcr p15, 0, %0, c1, c0,  0" : : "r" (Control) : "memory");

	// invalidate unified TLB (if MMU is re-enabled later)
	asm volatile("mcr p15, 0, %0, c8, c7,  0" : : "r" (0) : "memory");
	
	_PageTable(PageTable1);
	MemoryFree(PageTable1);
	PageTable1 = 0;
	
	_PageTable(PageTable0);
	MemoryFree(PageTable0);
	PageTable0 = 0;
}

u32 MemorySystemGetMemSize()
{
	return MemorySize;
}

void MemorySystemEnableMMU()
{
	u32 AuxControl;
	asm volatile("mrc p15, 0, %0, c1, c0,  1" : "=r" (AuxControl));
	#if RASPPI == 1
	AuxControl |= ARM_AUX_CONTROL_CACHE_SIZE;	// restrict cache size (no page coloring)
	#else
	AuxControl |= ARM_AUX_CONTROL_SMP;
#	endif
	asm volatile("mcr p15, 0, %0, c1, c0,  1" : : "r" (AuxControl));

	u32 TLBType;
	asm volatile("mrc p15, 0, %0, c0, c0,  3" : "=r" (TLBType));

	// set TTB control
	asm volatile("mcr p15, 0, %0, c2, c0,  2" : : "r" (TTBCR_SPLIT));

	// set TTBR0
	asm volatile("mcr p15, 0, %0, c2, c0,  0" : : "r" (PageTableGetBaseAddress (PageTable0)));

	// set TTBR1);
	asm volatile("mcr p15, 0, %0, c2, c0,  1" : : "r" (PageTableGetBaseAddress (PageTable1)));
	
	// set Domain Access Control register (Domain 0 and 1 to client)
	asm volatile("mcr p15, 0, %0, c3, c0,  0" : : "r" (DOMAIN_CLIENT << 0
							    | DOMAIN_CLIENT << 2));

	InvalidateDataCache();
	FlushPrefetchBuffer();

	// enable MMU
	u32 Control;
	asm volatile("mrc p15, 0, %0, c1, c0,  0" : "=r" (Control));
	#if RASPPI == 1
	#ifdef ARM_STRICT_ALIGNMENT
	Control &= ~ARM_CONTROL_UNALIGNED_PERMITTED;
	Control |= ARM_CONTROL_STRICT_ALIGNMENT;
	#else
	Control &= ~ARM_CONTROL_STRICT_ALIGNMENT;
	Control |= ARM_CONTROL_UNALIGNED_PERMITTED;
	#endif
	#endif
	Control |= MMU_MODE;
	asm volatile("mcr p15, 0, %0, c1, c0,  0" : : "r" (Control) : "memory");
}
