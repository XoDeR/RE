// Copyright (c) 2015 Volodymyr Syvochka
#pragma once

#include "Core/Base/Config.h"

#include "Core/Base/Types.h"
#include "Core/Debug/Error.h" // RIO_ASSERT

#include "Core/Memory/Allocator.h"
#include "Core/Thread/Mutex.h"

namespace Rio
{
	namespace MemoryFn
	{

// An allocator that uses the default system malloc(). Allocations are padded
// so that we can store the size of each allocation and align them to the desired alignment.
class HeapAllocator : public Allocator
{
public:
	HeapAllocator();
	~HeapAllocator();
	void* allocate(size_t size, size_t align = Allocator::defaultAlign);
	void deallocate(void* data);
	size_t getAllocatedSize(const void* ptr);
	size_t getTotalAllocated();
private:
	Mutex mutex;
	size_t totalAllocated;
	uint32_t allocationCount;
};

	} // namespace MemoryFn
} // namespace Rio