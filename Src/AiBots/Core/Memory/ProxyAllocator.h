// Copyright (c) 2015 Volodymyr Syvochka
#pragma once

#include "Core/Base/Config.h"
#include "Core/Memory/Allocator.h"

namespace Rio 
{

// Offers a facility to tag allocators by a string identifier.
// Proxy allocator is appended to a global linked list when instantiated
// so that it is possible to later visit that list for debugging purposes.
class ProxyAllocator : public Allocator
{
public:
	// Tag all allocations with the given name
	ProxyAllocator(const char* name, Allocator& allocator);
	void* allocate(size_t size, size_t align = Allocator::defaultAlign);
	void deallocate(void* data);
	size_t getAllocatedSize(const void* ptr) 
	{ 
		return sizeNotTracked; 
	}
	size_t getTotalAllocated() 
	{ 
		return sizeNotTracked; 
	}
	// Returns the name of the proxy allocator
	const char* getName() const;
private:
	const char* proxyName;
	Allocator& backingAllocator;
};

} // namespace Rio
