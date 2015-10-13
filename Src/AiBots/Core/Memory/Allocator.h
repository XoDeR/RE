// Copyright (c) 2015 Volodymyr Syvochka
#pragma once

#include "Core/Base/Config.h"
#include "Core/Base/Types.h"

#include <new>
#include <utility>

namespace Rio 
{

// Base class for memory allocators.
// Why using virtual functions:
// the cost of the virtual functions is much smaller than the memory allocation itself
class Allocator
{
public:
	// Default memory alignment in bytes
	static const size_t defaultAlign = sizeof(size_t);
	static const size_t sizeNotTracked = (size_t)(-1);

	Allocator() {}
	virtual	~Allocator() {}
	// Allocates size bytes of memory aligned to the specified
	// alignment and returns a pointer to the first allocated byte.
	virtual void* allocate(size_t size, size_t align = defaultAlign) = 0;
	virtual void deallocate(void* ptr) = 0;
	// Returns the size of the memory block pointed by ptr or sizeNotTracked
	// if the allocator does not support memory tracking.
	// the pointer must be a pointer returned by Allocator::allocate()
	// and allocated by the same allocator
	virtual size_t getAllocatedSize(const void* ptr) = 0;
	// Total number of allocated bytes
	virtual size_t getTotalAllocated() = 0;

	// Allocates and constructs type T
	// non-POD types only
	template <typename T, typename... Args>
	T* makeNew(Args&&... args)
	{
		return new (allocate(sizeof(T), RIO_ALIGNOF(T))) T{std::forward<Args>(args)...};
	}

	// Destructs and deallocates
	// non-POD types only
	template <typename T>
	void makeDelete(T* ptr)
	{
		if (ptr != nullptr)
		{
			ptr->~T();
			deallocate(ptr);
		}
	}
private:
	// Disable copying
	Allocator(const Allocator&);
	Allocator& operator=(const Allocator&);
};

} // namespace Rio
