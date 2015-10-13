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

// An allocator used to allocate temporary (scratch) memory.
// ScratchAllocator uses a fixed size ring buffer.
// If the ring buffer is exhausted, the scratch allocator will use
// its backing allocator to allocate memory instead.
class ScratchAllocator : public Allocator
{
public:
	ScratchAllocator(Allocator& backing, size_t size);
	~ScratchAllocator();
	bool inUse(const void* ptr);
	virtual void* allocate(size_t size, size_t align = Allocator::defaultAlign);
	virtual void deallocate(void* ptr);
	virtual size_t getAllocatedSize(const void* ptr);
	virtual size_t getTotalAllocated();
private:
	Allocator& backingAllocator;

	uint8_t* bufferBegin;
	uint8_t* bufferEnd;

	uint8_t* ptrAllocate;
	uint8_t* ptrFree;
};

} // namespace MemoryFn
} // namespace Rio