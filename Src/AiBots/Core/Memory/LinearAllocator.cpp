// Copyright (c) 2015 Volodymyr Syvochka
#include "LinearAllocator.h"

#include "Core/Memory/Memory.h"

namespace Rio 
{

LinearAllocator::LinearAllocator(Allocator& backing, size_t size)
	: backingAllocator(&backing)
	, physicalStart(nullptr)
	, totalSize(size)
	, offset(0)
{
	physicalStart = backing.allocate(size);
}

LinearAllocator::LinearAllocator(void* start, size_t size)
	: backingAllocator(NULL)
	, physicalStart(start)
	, totalSize(size)
	, offset(0)
{
}

LinearAllocator::~LinearAllocator()
{
	if (backingAllocator)
	{
		backingAllocator->deallocate(physicalStart);
	}
	RIO_ASSERT(this->offset == 0, "Memory leak of %ld bytes, maybe you forgot to call clear()?", this->offset);
}

void* LinearAllocator::allocate(size_t size, size_t align)
{
	const size_t actualSize = size + align;

	// Memory exhausted
	if (this->offset + actualSize > totalSize)
	{
		return NULL;
	}

	void* userPtr = MemoryFn::getAlignTop((char*)this->physicalStart + this->offset, align);
	this->offset += actualSize;
	return userPtr;
}

void LinearAllocator::deallocate(void* data)
{
	RIO_ASSERT(false, "Single deallocations not supported. Use clear()");
}

void LinearAllocator::clear()
{
	offset = 0;
}

} // namespace Rio
