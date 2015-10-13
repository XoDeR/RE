// Copyright (c) 2015 Volodymyr Syvochka
#pragma once

#include "Core/Base/Config.h"

#include "Core/Base/Types.h"
#include "Core/Debug/Error.h" // RIO_ASSERT

#include "Core/Memory/Allocator.h"

namespace Rio
{
	// Holds the number of bytes of an allocation
	struct Header
	{
		size_t size;
		// Value used to fill unused memory
		static const size_t padValue = (size_t)(-1);
	};

	inline void pad(Header* header, void* data)
	{
		size_t* ptr = (size_t*)(header + 1);

		while (ptr != data)
			*ptr++ = Header::padValue;
	}

	inline Header* header(const void* data)
	{
		const size_t* ptr = (size_t*)data;
		ptr--;

		while (*ptr == Header::padValue)
			ptr--;

		return (Header*)ptr;
	}

} // namespace Rio