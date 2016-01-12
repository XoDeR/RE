// Copyright (c) 2015 Volodymyr Syvochka
#pragma once

#include "Core/Base/Config.h"
#include "Core/Base/Types.h"

#include "Core/Debug/Error.h" // RIO_ASSERT

#include "Core/Memory/Memory.h"
#include "Core/Memory/Allocator.h"

#include "Core/Containers/Array.h"

#include <cstring> // memcpy

namespace Rio
{

	// Circular buffer double-ended queue of POD items
	// Does not call ctor/dtor
	// This allows for the use of memmove
	template <typename T>
	struct Queue
	{
		Queue(Allocator& allocator);
		T& operator[](size_t index);
		const T& operator[](size_t index) const;
		size_t offset;
		size_t size;
		Array<T> innerQueueData;

		ALLOCATOR_AWARE;
	};

	// Functions to manipulate Queue.
	namespace QueueFn
	{
		template<typename T> bool getIsEmpty(const Queue<T>& q);
		template<typename T> size_t getCount(const Queue<T>& q);

		// Returns the number of items the queue can hold before a resize must occur.
		template<typename T> size_t getSpace(const Queue<T>& q);

		// Increase/decrease the capacity of the queue.
		// Old items will be copied to the newly created queue.
		// If the new capacity is smaller than the previous one, the
		// queue will be truncated.
		template<typename T> void setCapacity(Queue<T>& q, size_t capacity);
		// Reserves space in the queue for at least capacity items
		template <typename T> void reserve(Queue<T>& q, size_t capacity);

		// Grows the queue to contain at least minCapacity items.
		// If minCapacity is set to 0, the queue automatically
		// determines the new capacity based on its size at the time of call.
		template<typename T> void grow(Queue<T>& q, size_t minCapacity);

		// Appends an item to the back of the queue
		template<typename T> void pushBack(Queue<T>& q, const T& item);
		// Removes the last item from the queue
		template<typename T> void popBack(Queue<T>& q);
		// Appends an item to the front of the queue
		template<typename T> void pushFront(Queue<T>& q, const T& item);
		// Removes the first item from the queue
		template<typename T> void popFront(Queue<T>& q);
		// Appends items to the back of the queue
		template<typename T> void push(Queue<T>& q, const T *items, size_t n);
		// Removes n items from the front of the queue
		template<typename T> void pop(Queue<T>& q, size_t n);

		// Does not free memory nor call dtors, only zeroes
		// the number of items in the queue for efficiency.
		template<typename T> void clear(Queue<T>& q);

		// Iterators
		// These iterators only go from the Queue::offset to the
		// end(Queue::data). end(q) - begin(q) may not equal getCount(q) because it is a ring
		// buffer. If you need to iterate over all the elements, either use
		// Queue::operator[](size_t index) or iterate over Queue::data
		template<typename T> T* begin(Queue<T>& q);
		template<typename T> const T* begin(const Queue<T>& q);
		template<typename T> T* end(Queue<T>& q);
		template<typename T> const T* end(const Queue<T>& q);

		template<typename T> T& front(Queue<T>& q);
		template<typename T> const T& front(const Queue<T>& q);
		template<typename T> T& back(Queue<T>& q);
		template<typename T> const T& back(const Queue<T>& q);
	} // namespace queue

	namespace QueueFn
	{
		template <typename T>
		inline bool getIsEmpty(const Queue<T>& q)
		{
			return q.size == 0;
		}

		template <typename T>
		inline size_t getCount(const Queue<T>& q)
		{
			return q.size;
		}

		template <typename T>
		inline size_t getSpace(const Queue<T>& q)
		{
			return ArrayFn::getCount(q.innerQueueData) - q.size;
		}

		template <typename T>
		inline void setCapacity(Queue<T>& q, size_t capacity)
		{
			size_t oldSize = ArrayFn::getCount(q.innerQueueData);
			ArrayFn::resize(q.innerQueueData, capacity);

			if (q.offset + q.size > oldSize)
			{
				memmove(ArrayFn::begin(q.innerQueueData) + capacity - (oldSize - q.offset),
					ArrayFn::begin(q.innerQueueData) + q.offset,
					(oldSize - q.offset) * sizeof(T));
				q.offset += (capacity - oldSize);
			}
		}

		template <typename T>
		inline void reserve(Queue<T>& q, size_t capacity)
		{
			if (capacity > q.size)
			{
				setCapacity(q, capacity);
			}
		}

		template <typename T>
		inline void grow(Queue<T>& q, size_t minCapacity)
		{
			size_t newCapacity = ArrayFn::getCount(q.innerQueueData) * 2 + 1;

			if (newCapacity < minCapacity)
			{
				newCapacity = minCapacity;
			}

			setCapacity(q, newCapacity);
		}

		template <typename T>
		inline void pushBack(Queue<T>& q, const T& item)
		{
			if (getSpace(q) == 0)
			{
				grow(q, 0);
			}

			q[q.size] = item;

			++(q.size);
		}

		template <typename T>
		inline void popBack(Queue<T>& q)
		{
			RIO_ASSERT(q.size > 0, "The queue is empty");
			--(q.size);
		}

		template <typename T>
		inline void pushFront(Queue<T>& q, const T& item)
		{
			if (getSpace(q) == 0)
			{
				grow(q, 0);
			}

			q.offset = (q.offset - 1 + ArrayFn::getCount(q.innerQueueData)) % ArrayFn::getCount(q.innerQueueData);
			q[0] = item;
			++(q.size);
		}

		template <typename T>
		inline void popFront(Queue<T>& q)
		{
			RIO_ASSERT(q.size > 0, "The queue is empty");

			q.offset = (q.offset + 1) % ArrayFn::getCount(q.innerQueueData);
			--(q.size);
		}

		template <typename T>
		inline void push(Queue<T>& q, const T *items, size_t n)
		{
			if (q.getSpace() < n)
			{
				q.grow(q.size + n);
			}

			const size_t size = ArrayFn::getCount(q.innerQueueData);
			const size_t insert = (q.offset + q.size) % size;

			size_t toInsert = n;
			if (insert + toInsert > size)
			{
				toInsert = size - insert;
			}

			memcpy(ArrayFn::begin(q.innerQueueData) + insert, items, toInsert * sizeof(T));

			q.size += toInsert;
			items += toInsert;
			n -= toInsert;
			memcpy(ArrayFn::begin(q.innerQueueData), items, n * sizeof(T));

			q.size += n;
		}

		template <typename T>
		inline void pop(Queue<T>& q, size_t n)
		{
			RIO_ASSERT(q.size > 0, "The queue is empty");
			q.offset = (q.offset + n) % ArrayFn::getCount(q.innerQueueData);
			q.size -= n;
		}

		template <typename T>
		inline void clear(Queue<T>& q)
		{
			q.offset = 0;
			q.size = 0;
		}

		template <typename T>
		inline T* begin(Queue<T>& q)
		{
			return ArrayFn::begin(q.innerQueueData) + q.offset;
		}

		template <typename T>
		inline const T* begin(const Queue<T>& q)
		{
			return ArrayFn::begin(q.innerQueueData) + q.offset;
		}

		template <typename T>
		inline T* end(Queue<T>& q)
		{
			size_t end = q.offset + q.size;
			return end >= ArrayFn::getCount(q.innerQueueData) ? ArrayFn::end(q.innerQueueData) : ArrayFn::begin(q.innerQueueData) + end;
		}

		template <typename T>
		inline const T* end(const Queue<T>& q)
		{
			size_t end = q.offset + q.size;
			return end >= ArrayFn::getCount(q.innerQueueData) ? ArrayFn::end(q.innerQueueData) : ArrayFn::begin(q.innerQueueData) + end;
		}

		template <typename T>
		inline T& front(Queue<T>& q)
		{
			RIO_ASSERT(q.size > 0, "The queue is empty");
			return q.innerQueueData[q.offset];
		}

		template <typename T>
		inline const T& front(const Queue<T>& q)
		{
			RIO_ASSERT(q.size > 0, "The queue is empty");
			return q.innerQueueData[q.offset];
		}

		template <typename T>
		inline T& back(Queue<T>& q)
		{
			RIO_ASSERT(q.size > 0, "The queue is empty");
			return q[q.size - 1];
		}

		template <typename T>
		inline const T& back(const Queue<T>& q)
		{
			RIO_ASSERT(q.size > 0, "The queue is empty");
			return q[q.size - 1];
		}

	} // namespace queue

	template <typename T>
	inline Queue<T>::Queue(Allocator& allocator)
		: offset(0)
		, size(0)
		, innerQueueData(allocator)
	{
	}

	template <typename T>
	inline T& Queue<T>::operator[](size_t index)
	{
		return innerQueueData[(offset + index) % ArrayFn::getCount(innerQueueData)];
	}

	template <typename T>
	inline const T& Queue<T>::operator[](size_t index) const
	{
		return innerQueueData[(offset + index) % ArrayFn::getCount(innerQueueData)];
	}

} // namespace Rio
