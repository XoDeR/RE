// Copyright (c) 2015 Volodymyr Syvochka
#pragma once

#include "Core/Base/Config.h"

#include "Core/Memory/Allocator.h"

namespace Rio 
{
// Allocates memory linearly in a stack-like fashion from a predefined chunk
// All deallocations must occur in LIFO order.
class StackAllocator : public Allocator
{
public:
	StackAllocator(void* start, size_t size);
	~StackAllocator();
	void* allocate(size_t size, size_t align = Allocator::defaultAlign);
	// Deallocations must occur in LIFO order, the last allocation must be freed for first.
	void deallocate(void* data);
	size_t getAllocatedSize(const void* ptr)
	{ 
		return sizeNotTracked;
	}
	size_t getTotalAllocated();
private:
	struct Header
	{
		uint32_t offset;
		uint32_t allocId;
	};
	void* physicalStart;
	size_t totalSize;
	void* topPointer;
	uint32_t allocationCount;
};

} // namespace Rio
