// Copyright (c) 2015 Volodymyr Syvochka
#include "Core/Memory/HeapAllocator.h"
#include "Core/Memory/Memory.h"
#include "Core/Memory/MemoryAux.h"
#include "Core/Thread/ScopedMutex.h"

namespace Rio
{
namespace MemoryFn
{

HeapAllocator::HeapAllocator() 
	: totalAllocated(0)
	, allocationCount(0)
{

}

HeapAllocator::~HeapAllocator()
{
	RIO_ASSERT(allocationCount == 0 && getTotalAllocated() == 0,
		"Missing %d deallocations causing a leak of %ld bytes", allocationCount, getTotalAllocated());
}

void HeapAllocator::deallocate(void* data)
{
	ScopedMutex sm(mutex);

	if (!data)
	{
		return;
	}

	Header* h = header(data);

	totalAllocated -= h->size;
	allocationCount--;

	free(h);
}

size_t HeapAllocator::getAllocatedSize(const void* ptr)
{
	ScopedMutex sm(mutex);
	Header* h = header(ptr);
	return h->size;
}

size_t HeapAllocator::getTotalAllocated()
{
	ScopedMutex sm(mutex);
	return totalAllocated;
}

void* HeapAllocator::allocate(size_t size, size_t align)
{
	ScopedMutex sm(mutex);

	const size_t total = size + align + sizeof(Header);

	Header* header = (Header*)malloc(total);
	header->size = total;

	void* ptr = MemoryFn::getAlignTop(header + 1, align);

	pad(header, ptr);

	totalAllocated += total;
	allocationCount++;

	return ptr;
}

} // namespace MemoryFn
} // namespace Rio