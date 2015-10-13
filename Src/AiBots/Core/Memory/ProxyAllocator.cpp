// Copyright (c) 2015 Volodymyr Syvochka
#include "ProxyAllocator.h"

#include "Core/Debug/Error.h"

namespace Rio 
{

ProxyAllocator::ProxyAllocator(const char* name, Allocator& allocator)
	: proxyName(name)
	, backingAllocator(allocator)
{
	RIO_ASSERT(name != NULL, "Name must be != NULL");
}

void* ProxyAllocator::allocate(size_t size, size_t align)
{
	void* p = backingAllocator.allocate(size, align);
	return p;
}

void ProxyAllocator::deallocate(void* data)
{
	backingAllocator.deallocate(data);
}

const char* ProxyAllocator::getName() const
{
	return proxyName;
}

} // namespace Rio
