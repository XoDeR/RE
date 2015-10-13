// Copyright (c) 2015 Volodymyr Syvochka
#pragma once

#include "Core/Base/Config.h"
#include "Core/Memory/Allocator.h"
#include "Core/Memory/Memory.h"

namespace Rio 
{

// Allocates from a fixed-size buffer.
// When the internal memory is exhausted or the allocation size exceeds
// the remaining internal storage, the backing allocator is used instead.
// The memory is automatically freed when the allocator is destroyed.
template <size_t SIZE>
class TempAllocator : public Allocator
{
public:
	TempAllocator(Allocator& backing = getDefaultScratchAllocator());
	~TempAllocator();
	void* allocate(size_t size, size_t align = Allocator::defaultAlign);
	// Does nothing, the memory is automatically freed when the allocator is destroyed
	void deallocate(void* /*data*/) {}
	size_t getAllocatedSize(const void* /*ptr*/) { return sizeNotTracked; }
	size_t getTotalAllocated() { return sizeNotTracked; }
private:
	Allocator& backingAllocator;

	char* bufferBegin;
	char* bufferEnd;
	char* bufferCurrentPtr;
	size_t allocChunkSize;
	char buffer[SIZE];
};

// To reduce unneeded template instantiation
// pre-made sizes could be used. For blobs more than 4K use scratch allocator or heap allocator
using TempAllocator64 = TempAllocator<64>;
using TempAllocator128 = TempAllocator<128>;
using TempAllocator256 = TempAllocator<256>;
using TempAllocator512 = TempAllocator<512>;
using TempAllocator1024 = TempAllocator<1024>;
using TempAllocator2048 = TempAllocator<2048>;
using TempAllocator4096 = TempAllocator<4096>;

template <size_t SIZE>
inline TempAllocator<SIZE>::TempAllocator(Allocator& backing)
	: backingAllocator(backing)
	, bufferBegin(&buffer[0])
	, bufferCurrentPtr(&buffer[0])
	, bufferEnd(&buffer[SIZE - 1])
	, allocChunkSize(4 * 1024) // 4K
{
	*(void**)bufferBegin = 0;
	bufferCurrentPtr += sizeof(void*);
}

template <size_t SIZE>
inline TempAllocator<SIZE>::~TempAllocator()
{
	void* ptr = *(void**)buffer;
	while (ptr)
	{
		void* nextPtr = *(void**)ptr;
		backingAllocator.deallocate(ptr);
		ptr = nextPtr;
	}
}

template <size_t SIZE>
inline void* TempAllocator<SIZE>::allocate(size_t size, size_t align)
{
	bufferCurrentPtr = (char*)MemoryFn::getAlignTop(bufferCurrentPtr, align);

	if (size > size_t(bufferEnd - bufferCurrentPtr))
	{
		uint32_t toAllocate = size + align + sizeof(void*);

		if (toAllocate < allocChunkSize)
		{
			toAllocate = allocChunkSize;
		}

		allocChunkSize *= 2;

		void*ptr = backingAllocator.allocate(toAllocate);
		*(void **)bufferBegin = ptr;
		bufferCurrentPtr = bufferBegin = (uint8_t*)ptr;
		bufferEnd = bufferBegin + toAllocate;
		*(void**)bufferBegin = nullptr;
		bufferCurrentPtr += sizeof(void*);
		bufferCurrentPtr = (uint8_t*)MemoryFn::getAlignTop(bufferCurrentPtr, align);
	}

	void *result = bufferCurrentPtr;
	bufferCurrentPtr += size;
	return result;
}

} // namespace Rio
