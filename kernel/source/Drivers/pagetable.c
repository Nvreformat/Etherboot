#include <Etherboot/pagetable.h>
#include <Etherboot/alloc.h>
#include <Etherboot/sysconfig.h>
#include <Etherboot/sysutils.h>

#if RASPPI == 1
#define SDRAM_SIZE_MBYTE	512

#define TTBR_MODE	(  ARM_TTBR_INNER_CACHEABLE		\
			 | ARM_TTBR_OUTER_NON_CACHEABLE)
#else
#define SDRAM_SIZE_MBYTE	1024

#define TTBR_MODE	(  ARM_TTBR_INNER_WRITE_BACK		\
			 | ARM_TTBR_OUTER_WRITE_BACK)
#endif

void PageTable(TPageTable* This)
{
	This->TableAllocated = FALSE;
	This->Table = (TARMV6MMU_LEVEL1_SECTION_DESCRIPTOR*) MEM_PAGE_TABLE1;

	for (unsigned EntryIndex = 0; EntryIndex < 4096; EntryIndex++)
	{
		u32 nBaseAddress = MEGABYTE * EntryIndex;
		
		TARMV6MMU_LEVEL1_SECTION_DESCRIPTOR* Entry = &This->Table[EntryIndex];

		Entry->Value10	= 2;
		Entry->BBit = 1;
		Entry->CBit = 0;
		Entry->XNBit = 0;
		Entry->Domain = 0;
		Entry->IMPBit = 0;
		Entry->AP = AP_SYSTEM_ACCESS;
		Entry->TEX = 0;
		Entry->APXBit = APX_RW_ACCESS;
		Entry->SBit = 1;
		Entry->NGBit = 0;
		Entry->Value0 = 0;
		Entry->SBZ = 0;
		Entry->Base	= ARMV6MMUL1SECTIONBASE(nBaseAddress);

		if (EntryIndex >= SDRAM_SIZE_MBYTE)
		{
			Entry->XNBit = 1;
		}
	}

	CleanDataCache ();
	DataSyncBarrier ();
}

void PageTable2 (TPageTable *This, u32 MemSize)
{
	This->TableAllocated = TRUE;
	This->Table = (TARMV6MMU_LEVEL1_SECTION_DESCRIPTOR *) PageAlloc();

	for (unsigned EntryIndex = 0; EntryIndex < SDRAM_SIZE_MBYTE; EntryIndex++)
	{
		u32 BaseAddress = MEGABYTE * EntryIndex;

		TARMV6MMU_LEVEL1_SECTION_DESCRIPTOR* Entry = &This->Table[EntryIndex];

		Entry->Value10	= 2;
		Entry->BBit	= 1;
		Entry->CBit	= 1;
		Entry->XNBit = 0;
		Entry->Domain = 0;
		Entry->IMPBit = 0;
		Entry->AP = AP_SYSTEM_ACCESS;
		Entry->TEX = 0;
		Entry->APXBit = APX_RW_ACCESS;
		Entry->SBit	= 0;
		Entry->NGBit = 0;
		Entry->Value0 = 0;
		Entry->SBZ = 0;
		Entry->Base	= ARMV6MMUL1SECTIONBASE(BaseAddress);

		extern u8 _etext;
		if (BaseAddress >= (u32) &_etext)
		{
			Entry->XNBit = 1;

			if (BaseAddress >= MemSize)
			{
				Entry->BBit  = 1;
				Entry->CBit  = 0;
				Entry->TEX   = 0;
				Entry->SBit  = 1;
			}
		}
	}

	CleanDataCache ();
	DataSyncBarrier ();
}

void _PageTable(TPageTable *This)
{
	if (This->TableAllocated)
	{
		PageFree(This->Table);
		This->Table = 0;
	}
}

u32 PageTableGetBaseAddress (TPageTable* This)
{
	return (u32) This->Table | TTBR_MODE;
}
