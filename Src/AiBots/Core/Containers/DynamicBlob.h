// Copyright (c) 2015 Volodymyr Syvochka
#pragma once

#include "Core/Base/Config.h"

#include "Core/Memory/Allocator.h"
#include "Core/Containers/Array.h"

namespace Rio
{

	// Dynamic block of raw memory.
	typedef Array<char> DynamicBlob;

	// Functions to manipulate DynamicBlob.
	namespace DynamicBlobFn
	{
		size_t getCount(const DynamicBlob& b);

		// Returns a pointer to the byte at the given absolute offset into the blob b.
		template <typename T> const T* get(const DynamicBlob& b, uint32_t offset);

		template <typename T> T* get(DynamicBlob& b, uint32_t offset);

		// Appends num items to the blob b and returns the size of the
		// blob after the append operation.
		template <typename T> uint32_t push(DynamicBlob& b, const T* items, uint32_t num);

		// Appends a blob to b and returns the size of the blob after the append operation.
		uint32_t pushBlob(DynamicBlob& b, const DynamicBlob& other);
	} // namespace DynamicBlobFn

	namespace DynamicBlobFn
	{
		inline size_t getCount(const DynamicBlob& b)
		{
			return ArrayFn::getCount(b);
		}

		template <typename T>
		inline const T* get(const DynamicBlob& b, uint32_t offset)
		{
			RIO_ASSERT(offset < getCount(b), "Overflow (size = %d, offset = %d", getCount(b), offset);
			return (T*)ArrayFn::begin(b) + offset;
		}

		template <typename T>
		inline T* get(DynamicBlob& b, uint32_t offset)
		{
			RIO_ASSERT(offset < getCount(b), "Overflow (size = %d, offset = %d", getCount(b), offset);
			return (T*)ArrayFn::begin(b) + offset;
		}

		template <typename T>
		inline uint32_t push(DynamicBlob& b, const T* items, uint32_t num)
		{
			return ArrayFn::push(b, (const char*)items, num);
		}

		inline uint32_t pushBlob(DynamicBlob& b, const DynamicBlob& other)
		{
			return ArrayFn::push(b, ArrayFn::begin(other), getCount(other));
		}
	} // namespace DynamicBlobFn

} // namespace Rio
