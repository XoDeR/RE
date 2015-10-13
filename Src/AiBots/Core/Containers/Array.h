// Copyright (c) 2015 Volodymyr Syvochka
#pragma once

#include "Core/Base/Config.h"

#include "Core/Debug/Error.h"
#include "Core/Memory/Allocator.h"

#include "Core/Base/Types.h"

#include <cstring> // memcpy

namespace Rio 
{

	class Allocator;
	// Dynamic array for POD types
	// Does not call ctor/dtor
	// This allows for the use of memmove
	template <typename T>
	struct Array
	{
		Array(Allocator& allocator);
		Array(Allocator& allocator, size_t capacity);
		Array(const Array<T>& other);
		~Array();
		T& operator[](size_t index);
		const T& operator[](size_t index) const;
		Array<T>& operator=(const Array<T>& other);

		Allocator* allocator;
		size_t capacity;
		size_t size;
		T* innerArrayData;
	};

namespace ArrayFn
{
	template <typename T> bool getIsEmpty(const Array<T>& a);
	template <typename T> size_t getCount(const Array<T>& a);
	// Maximum items that array can hold
	template <typename T> size_t getCapacity(const Array<T>& a);
	// Resizes the array to the given size.
	// Old items will be copied to the newly created array.
	// If the new capacity is smaller than the previous one, the array will be truncated.
	template <typename T> void resize(Array<T>& a, size_t size);
	// Reserves space for at least capacity items.
	template <typename T> void reserve(size_t capacity);
	template <typename T> void setCapacity(Array<T>& a, size_t capacity);
	// Grows the array to contain at least minCapacity items.
	template <typename T> void grow(Array<T>& a, size_t minCapacity);
	// Condenses the array so that its capacity matches the actual number
	// of items in the array
	template <typename T> void condense(Array<T>& a);
	// Appends an item to the array a and returns its index.
	template <typename T> size_t pushBack(Array<T>& a, const T& item);
	// Removes the last item from the array
	template <typename T> void popBack(Array<T>& a);
	// Appends count items to the array a and returns the number
	// of items in the array after the append operation.
	template <typename T> size_t push(Array<T>& a, const T* items, size_t count);
	// Clears the content of the array 
	// Does not free memory nor call dtors, it only zeroes
	// the number of items in the array.
	template <typename T> void clear(Array<T>& a);
	template <typename T> T* begin(Array<T>& a);
	template <typename T> const T* begin(const Array<T>& a);
	template <typename T> T* end(Array<T>& a);
	template <typename T> const T* end(const Array<T>& a);
	template <typename T> T& front(Array<T>& a);
	template <typename T> const T& front(const Array<T>& a);
	template <typename T> T& back(Array<T>& a);
	template <typename T> const T& back(const Array<T>& a);
} // namespace ArrayFn

namespace ArrayFn
{
	template <typename T>
	inline bool getIsEmpty(const Array<T>& a)
	{
		return a.size == 0;
	}

	template <typename T>
	inline size_t getCount(const Array<T>& a)
	{
		return a.size;
	}

	template <typename T>
	inline size_t getCapacity(const Array<T>& a)
	{
		return a.capacity;
	}

	template <typename T>
	inline void resize(Array<T>& a, size_t size)
	{
		if (size > a.capacity)
		{
			setCapacity(a, size);
		}
		a.size = size;
	}

	template <typename T>
	inline void reserve(Array<T>& a, size_t capacity)
	{
		if (capacity > a.capacity)
		{
			grow(a, capacity);
		}
	}

	template <typename T>
	inline void setCapacity(Array<T>& a, size_t capacity)
	{
		if (capacity == a.capacity)
		{
			return;
		}

		if (capacity < a.size)
		{
			resize(a, capacity);
		}

		T* data = nullptr;
		if (capacity > 0)
		{
			data = (T*)a.allocator.allocate(capacity * sizeof(T), RIO_ALIGNOF(T));
			memcpy(data, a.innerArrayData, a.size * sizeof(T));
		}
		a.allocator.deallocate(a.innerArrayData);
		a.innerArrayData = data;
		a.capacity = capacity;
	}

	template <typename T>
	inline void grow(Array<T>& a, size_t minCapacity)
	{
		size_t newCapacity = a.capacity * 2 + 1;
		if (newCapacity < minCapacity)
		{
			newCapacity = minCapacity;
		}
		setCapacity(a, newCapacity);
	}

	template <typename T>
	inline void condense(Array<T>& a)
	{
		resize(a, a.size);
	}

	template <typename T>
	inline size_t pushBack(Array<T>& a, const T& item)
	{
		if (a.capacity == a.size)
		{
			grow(a, 0);
		}
		a.innerArrayData[a.size] = item;
		return a.size++;
	}

	template <typename T>
	inline void popBack(Array<T>& a)
	{
		RIO_ASSERT(a.size > 0, "The array is empty");
		--a.size;
	}

	template <typename T>
	inline size_t push(Array<T>& a, const T* items, size_t count)
	{
		if (a.capacity <= a.size + count)
		{
			grow(a, a.size + count);
		}
		memcpy(&a.innerArrayData[a.size], items, sizeof(T) * count);
		a.size += count;
		return a.size;
	}

	template <typename T>
	inline void clear(Array<T>& a)
	{
		a.size = 0;
	}

	template <typename T>
	inline const T* begin(const Array<T>& a)
	{
		return a.innerArrayData;
	}

	template <typename T>
	inline T* begin(Array<T>& a)
	{
		return a.innerArrayData;
	}

	template <typename T>
	inline const T* end(const Array<T>& a)
	{
		return a.innerArrayData + a.size;
	}

	template <typename T>
	inline T* end(Array<T>& a)
	{
		return a.innerArrayData + a.size;
	}

	template <typename T>
	inline T& front(Array<T>& a)
	{
		RIO_ASSERT(a.size > 0, "The array is empty");
		return a.innerArrayData[0];
	}

	template <typename T>
	inline const T& front(const Array<T>& a)
	{
		RIO_ASSERT(a.size > 0, "The array is empty");
		return a.innerArrayData[0];
	}

	template <typename T>
	inline T& back(Array<T>& a)
	{
		RIO_ASSERT(a.size > 0, "The array is empty");
		return a.innerArrayData[a.size - 1];
	}

	template <typename T>
	inline const T& back(const Array<T>& a)
	{
		RIO_ASSERT(a.size > 0, "The array is empty");
		return a.innerArrayData[a.size - 1];
	}
} // namespace ArrayFn

template <typename T>
inline Array<T>::Array(Allocator& a)
	: allocator(&a)
	, capacity(0)
	, size(0)
	, innerArrayData(nullptr)
{
}

template <typename T>
inline Array<T>::Array(Allocator& a, size_t capacity)
	: allocator(&a)
	, capacity(0)
	, size(0)
	, innerArrayData(NULL)
{
	ArrayFn::resize(*this, capacity);
}

template <typename T>
inline Array<T>::Array(const Array<T>& other)
	: allocator(other.allocator)
	, capacity(0)
	, size(0)
	, innerArrayData(NULL)
{
	*this = other;
}

template <typename T>
inline Array<T>::~Array()
{
	if (innerArrayData)
	{
		allocator->deallocate(innerArrayData);
	}
}

template <typename T>
inline T& Array<T>::operator[](size_t index)
{
	RIO_ASSERT(index < this->size, "Index out of bounds");
	return innerArrayData[index];
}

template <typename T>
inline const T& Array<T>::operator[](size_t index) const
{
	RIO_ASSERT(index < this->size, "Index out of bounds");
	return innerArrayData[index];
}

template <typename T>
inline Array<T>& Array<T>::operator=(const Array<T>& other)
{
	const size_t size = other.size;
	ArrayFn::resize(*this, size);
	memcpy(this->innerArrayData, other.innerArrayData, sizeof(T) * size);
	return *this;
}

} // namespace Rio
