// Copyright (c) 2015 Volodymyr Syvochka
#include "Memory.h"

#include "Core/Memory/Allocator.h"
#include "Core/Thread/Mutex.h"
#include "Core/Memory/HeapAllocator.h"
#include "Core/Memory/ScratchAllocator.h"

#include <stdlib.h> // malloc, free

#define GLOBAL_NEW_DISABLED 0

#if GLOBAL_NEW_DISABLED
void* operator new(size_t) throw (std::bad_alloc)
{
	RIO_ASSERT(false, "operator new forbidden");

	return NULL;
}

void* operator new[](size_t) throw (std::bad_alloc)
{
	RIO_ASSERT(false, "operator new[] forbidden");

	return NULL;
}

void operator delete(void*) throw ()
{
	RIO_ASSERT(false, "operator delete forbidden");
}

void operator delete[](void*) throw ()
{
	RIO_ASSERT(false, "operator delete[] forbidden");
}
#endif // GLOBAL_NEW_DISABLED

namespace Rio 
{

namespace MemoryGlobalsFn
{
	namespace
	{
		using namespace MemoryFn;
		struct MemoryGlobals
		{
			static const size_t allocatorMemory = sizeof(HeapAllocator) + sizeof(ScratchAllocator);
			uint8_t buffer[allocatorMemory];

			HeapAllocator* defaultAllocator = nullptr;
			ScratchAllocator* defaultScratchAllocator = nullptr;
		};
		MemoryGlobals memoryGlobals;
	} // namespace (anonymous)

	void init()
	{
		using namespace MemoryFn;
		uint8_t* ptr = memoryGlobals.buffer;

		memoryGlobals.defaultAllocator = new (ptr)HeapAllocator;
		ptr += sizeof(HeapAllocator); // Move to next allocator
		// Set with 16K bytes of temporary memory
		memoryGlobals.defaultScratchAllocator = new (ptr)
			ScratchAllocator(*memoryGlobals.defaultAllocator, 16 * 1024);
	}

	void shutdown()
	{
		using namespace MemoryFn;
		// Destruct the scratch allocator first as its backing is the heap allocator
		memoryGlobals.defaultScratchAllocator->~ScratchAllocator();
		memoryGlobals.defaultAllocator->~HeapAllocator();
		// Set everything to null
		memoryGlobals = MemoryGlobals{};
	}
} // namespace MemoryGlobalsFn

Allocator& getDefaultAllocator()
{
	return *MemoryGlobalsFn::memoryGlobals.defaultAllocator;
}

Allocator& getDefaultScratchAllocator()
{
	return *MemoryGlobalsFn::memoryGlobals.defaultScratchAllocator;
}

} // namespace Rio
