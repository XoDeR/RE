// Copyright (c) 2015 Volodymyr Syvochka
#pragma once

#include "Core/Base/Config.h"
#include "Core/Memory/Allocator.h"

namespace Rio
{
	// Allocates fixed-size memory blocks from a fixed memory pool.
	// The backing allocator is used to allocate the memory pool.
	class PoolAllocator : public Allocator
	{
	public:
		// Uses backing to allocate the memory pool for containing exactly
		// blockCount blocks of blockSize size each aligned to blockAlign.
		PoolAllocator(Allocator& backing, size_t blockCount, size_t blockSize, size_t blockAlign = Allocator::defaultAlign);
		~PoolAllocator();
		// Allocates a block of memory from the memory pool.
		// The size and align must match those passed to PoolAllocator::PoolAllocator()
		void* allocate(size_t size, size_t align = Allocator::defaultAlign);
		void deallocate(void* data);
		size_t getAllocatedSize(const void* ptr)
		{
			return sizeNotTracked;
		}
		size_t getTotalAllocated();
	private:
		Allocator& backingAllocator;
		void* bufferStart;
		// first unused
		void* freelist;
		size_t blockSize;
		size_t blockAlign;
		size_t allocationCount;
		size_t totalAllocated;
	};

} // namespace Rio
