// Copyright (c) 2015 Volodymyr Syvochka
#include "Core/Memory/StackAllocator.h"
#include "Core/Memory/Memory.h"

namespace Rio 
{

StackAllocator::StackAllocator(void* start, size_t size)
	: physicalStart(start)
	, totalSize(size)
	, topPointer(start)
	, allocationCount(0)
{
}

StackAllocator::~StackAllocator()
{
	RIO_ASSERT(allocationCount == 0 && getTotalAllocated() == 0,
		"Missing %d deallocations causing a leak of %ld bytes", allocationCount, getTotalAllocated());
}

void* StackAllocator::allocate(size_t size, size_t align)
{
	const size_t actual_size = sizeof(Header) + size + align;

	// Memory exhausted
	if ((char*)topPointer + actual_size > (char*)physicalStart + totalSize)
	{
		return nullptr;
	}

	// The offset from TOS to the start of the buffer
	uint32_t offset = (char*)topPointer - (char*)physicalStart;

	// Align user data only, ignore header alignment
	topPointer = (char*)MemoryFn::getAlignTop((char*)topPointer + sizeof(Header), align) - sizeof(Header);

	Header* header = (Header*)topPointer;
	header->offset = offset;
	header->allocId = allocationCount;

	void* user_ptr = (char*)topPointer + sizeof(Header);
	topPointer = (char*)topPointer + actual_size;

	allocationCount++;

	return user_ptr;
}

void StackAllocator::deallocate(void* data)
{
	if (!data)
	{
		return;
	}

	Header* data_header = (Header*)((char*)data - sizeof(Header));

	RIO_ASSERT(data_header->allocId == allocationCount - 1, "Deallocations must occur in LIFO order");

	topPointer = (char*)physicalStart + data_header->offset;
	allocationCount--;
}

size_t StackAllocator::getTotalAllocated()
{
	return (char*)topPointer - (char*)physicalStart;
}

} // namespace Rio
