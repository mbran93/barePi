#include "includes/memory.h"
#include "includes/types.h"

void memoryInit() {
    allocated = 0;
    FirstAllocation= HEAP_END;
    FirstFreeAllocation = NULL;
}

void* MemoryReserve(unsigned int length, void* physicalAddress) {
	return physicalAddress;
}

void MemoryDeallocate(void* address) {
	struct HeapAllocation *Current, **CurrentAddress;

	CurrentAddress = &FirstAllocation;
	Current = FirstAllocation;

	while (Current != HEAP_END) {
		if (Current->Address == address) {
			allocated -= Current->Length;
			*CurrentAddress = Current->Next;
			Current->Next = FirstFreeAllocation;
			FirstFreeAllocation = Current;
//			LOG_DEBUGF("Platform: free(%#x) (%d/%d)\n", address, allocated, sizeof(Heap));
			return;
		}
		else {
			Current = Current->Next;
			CurrentAddress = &((*CurrentAddress)->Next);
		}
	}

//	LOG_DEBUGF("Platform: free(%#x) (%d/%d)\n", address, allocated, sizeof(Heap));
//	LOG("Platform: Deallocated memory that was never allocated. Ignored, but you should look into it.\n");
}

void* MemoryAllocate(unsigned int size) {
	struct HeapAllocation *Current, *Next;
	if (FirstFreeAllocation == NULL) {
//		LOG_DEBUG("Platform: First memory allocation, reserving 16KiB of heap, 256 entries.\n");
		MemoryReserve(sizeof(Heap), &Heap);
		MemoryReserve(sizeof(Allocations), &Allocations);

		FirstFreeAllocation = &Allocations[0];
	}

	size += (8 - (size & 7)) & 7; // Align to 8

	if (allocated + size > sizeof(Heap)) {
//		LOG("Platform: Out of memory! We should've had more heap space in platform.c.\n");
		return NULL;
	}

	if (FirstFreeAllocation == HEAP_END) {
//		LOG("Platform: Out of memory! We should've had more allocations in platform.c.\n");
		return NULL;
	}
	Current = FirstAllocation;

	while (Current != HEAP_END) {
		if (Current->Next != HEAP_END) {
			if ((unsigned int)Current->Next->Address - (unsigned int)Current->Address - Current->Length >= size) {
				FirstFreeAllocation->Address = (void*)((unsigned char*)Current->Address + Current->Length);
				FirstFreeAllocation->Length = size;
				Next = FirstFreeAllocation;
				if (Next->Next == NULL)
					if ((unsigned int)(FirstFreeAllocation + 1) < (unsigned int)((unsigned char*)Allocations + sizeof(Allocations)))
						FirstFreeAllocation = FirstFreeAllocation + 1;
					else
						FirstFreeAllocation = HEAP_END;
				else
					FirstFreeAllocation = Next->Next;
				Next->Next = Current->Next;
				Current->Next = Next;
				allocated += size;
//				LOG_DEBUGF("Platform: malloc(%#x) = %#x. (%d/%d)\n", size, Next->Address, allocated, sizeof(Heap));
				return Next->Address;
			}
			else
				Current = Current->Next;
		} else {
			if ((unsigned int)&Heap[sizeof(Heap)] - (unsigned int)Current->Next - Current->Length >= size) {
				FirstFreeAllocation->Address = (void*)((unsigned char*)Current->Address + Current->Length);
				FirstFreeAllocation->Length = size;
				Next = FirstFreeAllocation;
				if (Next->Next == NULL)
					if ((unsigned int)(FirstFreeAllocation + 1) < (unsigned int)((unsigned char*)Allocations + sizeof(Allocations)))
						FirstFreeAllocation = FirstFreeAllocation + 1;
					else
						FirstFreeAllocation = HEAP_END;
				else
					FirstFreeAllocation = Next->Next;
				Next->Next = Current->Next;
				Current->Next = Next;
				allocated += size;
//				LOG_DEBUGF("Platform: malloc(%#x) = %#x. (%d/%d)\n", size, Next->Address, allocated, sizeof(Heap));
				return Next->Address;
			}
			else {
//				LOG("Platform: Out of memory! We should've had more heap space in platform.c.\n");
//				LOG_DEBUGF("Platform: malloc(%#x) = %#x. (%d/%d)\n", size, NULL, allocated, sizeof(Heap));
				return NULL;
			}
		}
	}

	Next = FirstFreeAllocation->Next;
	FirstAllocation = FirstFreeAllocation;
	FirstAllocation->Next = HEAP_END;
	FirstAllocation->Length = size;
	FirstAllocation->Address = &Heap;
	if (Next == NULL)
		if ((unsigned int)(FirstFreeAllocation + 1) < (unsigned int)((unsigned char*)Allocations + sizeof(Allocations)))
			FirstFreeAllocation = FirstFreeAllocation + 1;
		else
			FirstFreeAllocation = HEAP_END;
	else
		FirstFreeAllocation = Next;
	allocated += size;
//	LOG_DEBUGF("Platform: malloc(%#x) = %#x. (%d/%d)\n", size, FirstAllocation->Address, allocated, sizeof(Heap));
	return FirstAllocation->Address;
}