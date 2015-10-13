// Copyright (c) 2015 Volodymyr Syvochka
#pragma once

#include "Core/Base/Config.h"

#include "Core/Base/Types.h"
#include "Core/Debug/Error.h" // RIO_ASSERT

#include "Core/Memory/Allocator.h"

namespace Rio 
{

Allocator& getDefaultAllocator();
Allocator& getDefaultScratchAllocator();

namespace MemoryFn
{
	// Returns the pointer aligned to the desired align byte
	inline void* getAlignTop(void* ptr, size_t align)
	{
		RIO_ASSERT((align != 0) && !(align & (align - 1)), "Check if power of 2");

		uintptr_t p = (uintptr_t)ptr;
		const size_t mod = p % align;
		if (mod)
		{
			p += (uintptr_t)(align - mod);
		}
		return (void*)p;
	}
} // namespace MemoryFn

namespace MemoryGlobalsFn
{
	// Constructs the initial default allocators.
	// Has to be called before anything else during the engine startup.
	void init();
	// Destroys the allocators created with MemoryGlobalsFn::init().
	// Should be the last call of the program.
	void shutdown();
} // namespace MemoryGlobalsFn

} // namespace Rio
