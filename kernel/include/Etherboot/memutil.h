#ifndef UTIL_H
#define UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

void MemorySet(void* Addrress, register char Value, register int Size);
void* MemoryCopy(void* Source, void* Destination, register int Size);
int MemoryCompare(void* Source, void* Destination, register int Size);

#ifdef __cplusplus
}
#endif

#endif
