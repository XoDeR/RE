// Copyright (c) 2015 Volodymyr Syvochka
#pragma once

#include "Core/Base/Config.h"

#include "Core/Base/Types.h"
#include "Core/Debug/Error.h"

namespace Rio
{
	// Block of raw memory.
	struct Blob
	{
		uint32_t size;
		uintptr_t innerBlobData;
	};

	namespace BlobFn
	{
		uint32_t getCount(const Blob& b);
		// Returns a pointer to the byte at the given absolute offset into the blob
		template <typename T> const T* get(const Blob& b, uint32_t offset);
		template <typename T> T* get(Blob& b, uint32_t offset);
	} // namespace BlobFn

	namespace BlobFn
	{
		inline uint32_t getCount(const Blob& b)
		{
			return b.size;
		}

		template <typename T>
		inline const T* get(const Blob& b, uint32_t offset)
		{
			RIO_ASSERT(offset < b.size, "Overflow (size = %d, offset = %d", b.size, offset);
			return (T*)b.innerBlobData + offset;
		}

		template <typename T>
		inline T* get(Blob& b, uint32_t offset)
		{
			RIO_ASSERT(offset < b.size, "Overflow (size = %d, offset = %d", b.size, offset);
			return (T*)b.innerBlobData + offset;
		}
	} // namespace BlobFn

} // namespace Rio
