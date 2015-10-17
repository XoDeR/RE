// Copyright (c) 2015 Volodymyr Syvochka
#pragma once

#include "Core/Base/Config.h"

#include "Core/Debug/Error.h"
#include "Core/Memory/Allocator.h"

namespace Rio
{

	// Dynamic array
	// Calls ctor/dtor
	template <typename T>
	struct Vector
	{
		Vector(Allocator& allocator);
		Vector(Allocator& allocator, size_t capacity);
		Vector(const Vector<T>& other);
		~Vector();
		T& operator[](size_t index);
		const T& operator[](size_t index) const;
		const Vector<T>& operator=(const Vector<T>& other);
		Allocator* allocator;
		size_t capacity;
		size_t size;
		T* innerVectorData;
	};

	namespace VectorFn
	{
		template <typename T> bool getIsEmpty(const Vector<T>& v);
		template <typename T> size_t getCount(const Vector<T>& v);
		template <typename T> size_t getCapacity(const Vector<T>& v);
		// Resizes the vector v to the given size.
		// Old items will be copied to the newly created vector.
		// If the new capacity is smaller than the previous one, the
		// vector will be truncated.
		template <typename T> void resize(Vector<T>& v, size_t size);
		// Reserves space in the vector v for at least capacity items.
		template <typename T> void reserve(Vector<T>& v, size_t capacity);
		template <typename T> void setCapacity(Vector<T>& v, size_t capacity);
		// Grows the vector v to contain at least minCapacity items.
		template <typename T> void grow(Vector<T>& v, size_t minCapacity);
		// Condenses (shrinks) the vector so that its capacity matches the actual number
		// of items in the vector
		template <typename T> void condense(Vector<T>& v);
		// Appends an item to the vector and returns its index
		template <typename T> size_t pushBack(Vector<T>& v, const T& item);
		// Removes the last item from the vector
		template <typename T> void popBack(Vector<T>& v);
		// Appends count items to the vector and returns the number
		// of items in the vector after the append operation.
		template <typename T> size_t push(Vector<T>& v, const T* items, size_t count);
		// Clears the content of the vector
		// Calls destructor on the items.
		template <typename T> void clear(Vector<T>& v);

		template <typename T> T* begin(Vector<T>& v);
		template <typename T> const T* begin(const Vector<T>& v);
		template <typename T> T* end(Vector<T>& v);
		template <typename T> const T* end(const Vector<T>& v);

		template <typename T> T& front(Vector<T>& v);
		template <typename T> const T& front(const Vector<T>& v);
		template <typename T> T& back(Vector<T>& v);
		template <typename T> const T& back(const Vector<T>& v);
	} // namespace VectorFn

	namespace VectorFn
	{
		template <typename T>
		inline bool getIsEmpty(const Vector<T>& v)
		{
			return v.size == 0;
		}

		template <typename T>
		inline size_t getCount(const Vector<T>& v)
		{
			return v.size;
		}

		template <typename T>
		inline size_t getCapacity(const Vector<T>& v)
		{
			return v.capacity;
		}

		template <typename T>
		inline void resize(Vector<T>& v, size_t size)
		{
			if (size > v.capacity)
				setCapacity(v, size);

			v.size = size;
		}

		template <typename T>
		inline void reserve(Vector<T>& v, size_t capacity)
		{
			if (capacity > v.capacity)
			{
				grow(v, capacity);
			}
		}

		template <typename T>
		inline void setCapacity(Vector<T>& v, size_t capacity)
		{
			if (capacity == v.capacity)
			{
				return;
			}

			if (capacity < v.size)
			{
				resize(v, capacity);
			}

			if (capacity > 0)
			{
				T* tmp = v.innerVectorData;
				v.capacity = capacity;
				v.innerVectorData = (T*)v.allocator->allocate(capacity * sizeof(T), RIO_ALIGNOF(T));

				for (size_t i = 0; i < v.size; i++)
				{
					new (v.innerVectorData + i) T(tmp[i]);
				}

				if (tmp)
				{
					for (size_t i = 0; i < v.size; i++)
					{
						tmp[i].~T();
					}
					v.allocator->deallocate(tmp);
				}
			}
		}

		template <typename T>
		inline void grow(Vector<T>& v, size_t minCapacity)
		{
			size_t newCapacity = v.capacity * 2 + 1;

			if (newCapacity < minCapacity)
			{
				newCapacity = minCapacity;
			}

			setCapacity(v, newCapacity);
		}

		template <typename T>
		inline void condense(Vector<T>& v)
		{
			resize(v, v.size);
		}

		template <typename T>
		inline size_t pushBack(Vector<T>& v, const T& item)
		{
			if (v.capacity == v.size)
			{
				grow(v, 0);
			}

			new (v.innerVectorData + v.size) T(item);

			return v.size++;
		}

		template <typename T>
		inline void popBack(Vector<T>& v)
		{
			RIO_ASSERT(v.size > 0, "The vector is empty");
			v.innerVectorData[v.size - 1].~T();
			--v.size;
		}

		template <typename T>
		inline size_t push(Vector<T>& v, const T* items, size_t count)
		{
			if (v.capacity <= v.size + count)
			{
				grow(v, v.size + count);
			}

			T* arr = &v.innerVectorData[v.size];
			for (size_t i = 0; i < count; i++)
			{
				arr[i] = items[i];
			}

			v.size += count;
			return v.size;
		}

		template <typename T>
		inline void clear(Vector<T>& v)
		{
			for (size_t i = 0; i < v.size; i++)
			{
				v.innerVectorData[i].~T();
			}

			v.size = 0;
		}

		template <typename T>
		inline T* begin(Vector<T>& v)
		{
			return v.innerVectorData;
		}

		template <typename T>
		inline const T* begin(const Vector<T>& v)
		{
			return v.innerVectorData;
		}

		template <typename T>
		inline T* end(Vector<T>& v)
		{
			return v.innerVectorData + v.size;
		}

		template <typename T>
		inline const T* end(const Vector<T>& v)
		{
			return v.innerVectorData + v.size;
		}

		template <typename T>
		inline T& front(Vector<T>& v)
		{
			RIO_ASSERT(v.size > 0, "The vector is empty");
			return v.innerVectorData[0];
		}

		template <typename T>
		inline const T& front(const Vector<T>& v)
		{
			RIO_ASSERT(v.size > 0, "The vector is empty");
			return v.innerVectorData[0];
		}

		template <typename T>
		inline T& back(Vector<T>& v)
		{
			RIO_ASSERT(v.size > 0, "The vector is empty");
			return v.innerVectorData[v.size - 1];
		}

		template <typename T>
		inline const T& back(const Vector<T>& v)
		{
			RIO_ASSERT(v.size > 0, "The vector is empty");
			return v.innerVectorData[v.size - 1];
		}
	} // namespace VectorFn

	template <typename T>
	inline Vector<T>::Vector(Allocator& a)
		: allocator(&a)
		, capacity(0)
		, size(0)
		, innerVectorData(NULL)
	{
	}

	template <typename T>
	inline Vector<T>::Vector(Allocator& a, size_t capacity)
		: allocator(&a)
		, capacity(0)
		, size(0)
		, innerVectorData(NULL)
	{
		VectorFn::resize(*this, capacity);
	}

	template <typename T>
	inline Vector<T>::Vector(const Vector<T>& other)
		: allocator(other.allocator)
		, capacity(0)
		, size(0)
		, innerVectorData(NULL)
	{
		*this = other;
	}

	template <typename T>
	inline Vector<T>::~Vector()
	{
		for (size_t i = 0; i < size; i++)
		{
			innerVectorData[i].~T();
		}

		if (innerVectorData)
		{
			allocator->deallocate(innerVectorData);
		}
	}

	template <typename T>
	inline T& Vector<T>::operator[](size_t index)
	{
		RIO_ASSERT(index < size, "Index out of bounds");
		return innerVectorData[index];
	}

	template <typename T>
	inline const T& Vector<T>::operator[](size_t index) const
	{
		RIO_ASSERT(index < size, "Index out of bounds");
		return innerVectorData[index];
	}

	template <typename T>
	inline const Vector<T>& Vector<T>::operator=(const Vector<T>& other)
	{
		const size_t size = VectorFn::getCount(other);
		VectorFn::resize(*this, size);

		for (size_t i = 0; i < size; i++)
		{
			this->innerVectorData[i] = innerVectorData._array[i];
		}

		return *this;
	}

} // namespace Rio
