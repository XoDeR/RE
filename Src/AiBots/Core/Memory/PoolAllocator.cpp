// Copyright (c) 2015 Volodymyr Syvochka
#include "Core/Memory/PoolAllocator.h"
#include "Core/Debug/Error.h"

namespace Rio
{

	PoolAllocator::PoolAllocator(Allocator& backing, size_t blockCount, size_t blockSize, size_t blockAlign)
		: backingAllocator(backing)
		, bufferStart(nullptr)
		, freelist(NULL)
		, blockSize(blockSize)
		, blockAlign(blockAlign)
		, allocationCount(0)
		, totalAllocated(0)
	{
		RIO_ASSERT(blockCount > 0, "Unsupported number of blocks");
		RIO_ASSERT(blockSize > 0, "Unsupported block size");
		RIO_ASSERT(blockAlign > 0, "Unsupported block alignment");

		size_t actual_block_size = blockSize + blockAlign;
		size_t pool_size = blockCount * actual_block_size;

		char* mem = (char*)backing.allocate(pool_size, blockAlign);

		// Initialize intrusive freelist
		char* cur = mem;
		for (size_t bb = 0; bb < blockCount - 1; bb++)
		{
			uintptr_t* next = (uintptr_t*)cur;
			*next = (uintptr_t)cur + actual_block_size;
			cur += actual_block_size;
		}

		uintptr_t* end = (uintptr_t*)cur;
		*end = (uintptr_t)NULL;

		bufferStart = mem;
		freelist = mem;
	}

	PoolAllocator::~PoolAllocator()
	{
		backingAllocator.deallocate(bufferStart);
	}

	void* PoolAllocator::allocate(size_t size, size_t align)
	{
		RIO_ASSERT(size == blockSize, "Size must match block size");
		RIO_ASSERT(align == blockAlign, "Align must match block align");
		RIO_ASSERT(freelist != NULL, "Out of memory");

		uintptr_t next_free = *((uintptr_t*)freelist);
		void* userPtr = freelist;
		freelist = (void*)next_free;

		allocationCount++;
		totalAllocated += blockSize;

		return userPtr;
	}

	void PoolAllocator::deallocate(void* data)
	{
		if (!data)
			return;

		RIO_ASSERT(allocationCount > 0, "Did not allocate");

		uintptr_t* next = (uintptr_t*)data;
		*next = (uintptr_t)freelist;

		freelist = data;

		allocationCount--;
		totalAllocated -= blockSize;
	}

	size_t PoolAllocator::getTotalAllocated()
	{
		return totalAllocated;
	}

} // namespace Rio
