// Copyright (c) 2015 Volodymyr Syvochka
#pragma once

#include "Core/Base/Config.h"

#include "Core/Base/Types.h"

#include "Core/Debug/Error.h" // RIO_ASSERT

#include "Core/Memory/Memory.h"
#include "Core/Memory/Allocator.h"

#include "Core/Containers/Array.h"

#include <cstring> // memcpy
#include <algorithm>

namespace Rio
{
	// Priority queue
	// POD items
	// Does not call ctor/dtor
	template <typename T>
	struct PriorityQueue
	{
		PriorityQueue(Allocator& a);
		Array<T> innerPriorityQueueData;

		ALLOCATOR_AWARE;
	};

	namespace PriorityQueueFn
	{
		template <typename T> const T& getTop(const PriorityQueue<T>& q);
		template <typename T> void push(PriorityQueue<T>& q, const T& item);
		template <typename T> void pop(PriorityQueue<T>& q);
	} // namespace PriorityQueueFn

	namespace PriorityQueueFn
	{
		template <typename T>
		const T& getTop(const PriorityQueue<T>& q)
		{
			return ArrayFn::front(q.innerPriorityQueueData);
		}

		template <typename T>
		void push(PriorityQueue<T>& q, const T& item)
		{
			ArrayFn::pushBack(q.innerPriorityQueueData, item);
			std::push_heap(ArrayFn::begin(q.innerPriorityQueueData), ArrayFn::end(q.innerPriorityQueueData));
		}

		template <typename T>
		void pop(PriorityQueue<T>& q)
		{
			std::pop_heap(ArrayFn::begin(q.innerPriorityQueueData), ArrayFn::end(q.innerPriorityQueueData));
			ArrayFn::popBack(q.innerPriorityQueueData);
		}
	} // namespace PriorityQueueFn

	template <typename T>
	PriorityQueue<T>::PriorityQueue(Allocator& a)
		: innerPriorityQueueData(a)
	{
	}

} // namespace Rio
