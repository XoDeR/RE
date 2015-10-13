// Copyright (c) 2015 Volodymyr Syvochka
#pragma once

#include "Core/Base/Config.h"
#include "Core/Memory/Allocator.h"

namespace Rio 
{

// Allocates memory linearly from a predefined chunk
// and frees all the allocations with a single call to clear()
class LinearAllocator : public Allocator
{
public:
	LinearAllocator(Allocator& backing, size_t size);
	LinearAllocator(void* start, size_t size);
	~LinearAllocator();
	void* allocate(size_t size, size_t align = Allocator::defaultAlign);
	// The linear allocator does not support deallocating
	// individual allocations, rather you have to call
	// clear() to free all allocated memory at once.
	void deallocate(void* data);
	// Frees all the allocations made by allocate()
	void clear();
	size_t getAllocatedSize(const void* /*ptr*/)
	{ 
		return sizeNotTracked;
	}
	size_t getTotalAllocated()
	{ 
		return offset;
	}
private:
	Allocator* backingAllocator;
	void* physicalStart;
	size_t totalSize;
	size_t offset;
};

} // namespace Rio
