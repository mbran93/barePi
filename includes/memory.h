#ifndef __MEMORY_H__
#define __MEMORY_H__

#define HEAP_END ((void*)0xFFFFFFFF)

struct HeapAllocation {
	unsigned int Length;
	void* Address;
	struct HeapAllocation *Next;
};

unsigned char Heap[0x4000] __attribute__((aligned(8))); // Support a maximum of 16KiB of allocations
struct HeapAllocation Allocations[0x100]; // Support 256 allocations
struct HeapAllocation *FirstAllocation, *FirstFreeAllocation;
unsigned int allocated;

void memoryInit();
void *MemoryReserve(unsigned int, void *);
void MemoryDeallocate(void *);
void *MemoryAllocate(unsigned int);


#endif