// Copyright (c) 2015 Volodymyr Syvochka
#include "Core/Memory/ScratchAllocator.h"
#include "Core/Memory/Memory.h"
#include "Core/Memory/MemoryAux.h"
#include "Core/Thread/ScopedMutex.h"

namespace Rio
{
	namespace MemoryFn
	{
		ScratchAllocator::ScratchAllocator(Allocator& backing, size_t size) 
			: backingAllocator{ backing }
		{
			bufferBegin = (uint8_t*)backingAllocator.allocate(size);
			bufferEnd = bufferBegin + size;
			ptrAllocate = bufferBegin;
			ptrFree = bufferBegin;
		}

		ScratchAllocator::~ScratchAllocator()
		{
			RIO_ASSERT(ptrFree == ptrAllocate, "");
			backingAllocator.deallocate(bufferBegin);
		}

		bool ScratchAllocator::inUse(const void* ptr)
		{
			if (ptrAllocate == ptrFree)
				return false;
			if (ptrAllocate > ptrFree)
				return ptr >= ptrFree && ptr < ptrAllocate;

			return ptr >= ptrFree || ptr < ptrAllocate;
		}

		void* ScratchAllocator::allocate(size_t size, size_t align)
		{
			RIO_ASSERT(align % 4 == 0, "Align should be 4 or 8");
			size = ((size + 3) / 4) * 4; // Move size to next alignment block

			uint8_t* ptr = ptrAllocate;
			Header* h = (Header*)ptr;
			uint8_t* data = (uint8_t*)getAlignTop(h + 1, align);
			ptr = data + size;

			// Reached the end of the buffer, wrap around to the beginning
			if (ptr > bufferEnd)
			{
				h->size = (bufferEnd - (uint8_t*)h) | ((size_t)(-1));

				ptr = bufferBegin;
				h = (Header*)ptr;
				data = (uint8_t*)getAlignTop(h + 1, align);
				ptr = data + size;
			}

			// If the buffer is exhausted use the backing allocator
			if (inUse(ptr))
				return backingAllocator.allocate(size, align);

			h->size = ptr - (uint8_t*)h;
			pad(h, data);

			ptrAllocate = ptr;

			return data;
		}

		void ScratchAllocator::deallocate(void* ptr)
		{
			if (!ptr)
				return;

			if (ptr < bufferBegin || ptr >= bufferEnd)
			{
				backingAllocator.deallocate(ptr);
				return;
			}

			// Set slot to be free
			Header* h = header(ptr);
			RIO_ASSERT((int64_t)h->size >= 0, "");
			h->size = h->size | ((size_t)(-1));

			// Move the free pointer past all the free slots
			while (ptrFree != ptrAllocate)
			{
				Header* h = (Header*)ptrFree;
				if ((int64_t)(h->size) < 0)
					break;

				// Loop back
				ptrFree += h->size;
				if (ptrFree == bufferEnd)
					ptrFree = bufferBegin;
			}
		}

		size_t ScratchAllocator::getAllocatedSize(const void* ptr)
		{
			const Header* h = header(ptr);
			return h->size - ((const uint8_t*)ptr - (const uint8_t*)h);
		}

		size_t ScratchAllocator::getTotalAllocated()
		{
			return bufferEnd - bufferBegin;
		}

	} // namespace MemoryFn
} // namespace Rio