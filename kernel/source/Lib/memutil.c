#include <Etherboot/memutil.h>

void MemorySet(void* Addrress, register char Value, register int Size)
{
	register char* Dest = (char*) Addrress;
	
	while(Size--)
	{
		*Dest++ = Value;
	}
}

void* __attribute__((optimize("O3"))) MemoryCopy(void* Source, void* Destination, register int Size)
{
	register char* DestPtr = (char*) Destination;
	register char* SrcPtr = (char*) Source;
	
	while(Size--)
	{
		*DestPtr++ = *SrcPtr++;
	}
	
	return Destination;
}

int MemoryCompare(void* Source, void* Destination, register int Size)
{
	register char* DestPtr = (char*) Destination;
	register char* SrcPtr = (char*) Source;
	
	while(Size--)
	{
		if (*DestPtr++ != *SrcPtr++)
			return 0;
	}
	
	return 1;
}
