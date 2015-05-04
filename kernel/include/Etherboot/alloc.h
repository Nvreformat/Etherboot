#ifndef ALLOC_H
#define ALLOC_H

#define MEM_PAGE_ALLOC

#ifdef __cplusplus
extern "C" {
#endif

void MemoryInitialize(unsigned long Base, unsigned long Size);

unsigned long MemoryGetSize(void);

void* MemoryAllocate(unsigned long Size);
void MemoryFree(void* Block);

#ifdef MEM_PAGE_ALLOC

void* PageAlloc(void);
void PageFree(void* Page);

#endif

#ifdef MEM_DEBUG

void MemoryGetInfo(void);

#endif

#ifdef __cplusplus
}
#endif

#endif
