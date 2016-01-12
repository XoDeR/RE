// Copyright (c) 2015 Volodymyr Syvochka
#pragma once

#include "Core/Base/Config.h"
#include "Core/Debug/Error.h"
#include "Core/Base/Types.h"
#include "Core/Base/Id.h"

namespace Rio 
{

// Packed array of objects with lookup table.
template <uint32_t MAX, typename T>
struct IdArray
{
	// Creates the table for tracking exactly MAX - 1 unique Ids.
	IdArray();
	T& operator[](uint32_t i);
	const T& operator[](uint32_t i) const;
	// The index of the first unused id
	uint16_t freelist;
	// Next available unique id
	uint16_t nextId;
	uint16_t size;
	// The last valid id is reserved and cannot be used to
	// refer to Ids from the outside
	Id sparseList[MAX];
	uint16_t sparseToDenseTable[MAX];
	uint16_t denseToSparseTable[MAX];
	T innerIdArrayData[MAX];
};

namespace IdArrayFn
{
	template <uint32_t MAX, typename T> Id create(IdArray<MAX, T>& a, const T& object);
	template <uint32_t MAX, typename T> void destroy(IdArray<MAX, T>& a, Id id);
	template <uint32_t MAX, typename T> bool has(const IdArray<MAX, T>& a, Id id);
	template <uint32_t MAX, typename T> uint32_t getCount(const IdArray<MAX, T>& a);
	template <uint32_t MAX, typename T> T& get(const Id& id);

	template <uint32_t MAX, typename T> T* begin(IdArray<MAX, T>& a);
	template <uint32_t MAX, typename T> const T* begin(const IdArray<MAX, T>& a);
	template <uint32_t MAX, typename T> T* end(IdArray<MAX, T>& a);
	template <uint32_t MAX, typename T> const T* end(const IdArray<MAX, T>& a);
} // namespace IdArrayFn

namespace IdArrayFn
{
	template <uint32_t MAX, typename T>
	inline Id create(IdArray<MAX, T>& a, const T& object)
	{
		RIO_ASSERT(a.size < MAX, "Object list full");

		// Obtain a new id
		Id id;
		id.id = a.nextId++;

		// Recycle slot if there are any
		if (a.freelist != Id::idInvalid)
		{
			id.index = a.freelist;
			a.freelist = a.sparseList[a.freelist].index;
		}
		else
		{
			id.index = a.size;
		}

		a.sparseList[id.index] = id;
		a.sparseToDenseTable[id.index] = a.size;
		a.denseToSparseTable[a.size] = id.index;
		a.innerIdArrayData[a.size] = object;
		a.size++;

		return id;
	}

	template <uint32_t MAX, typename T>
	inline void destroy(IdArray<MAX, T>& a, Id id)
	{
		RIO_ASSERT(has(a, id), "IdArray does not have ID: %d,%d", id.id, id.index);

		a.sparseList[id.index].id = Id::idInvalid;
		a.sparseList[id.index].index = a.freelist;
		a.freelist = id.index;

		// Swap with last element
		const uint32_t last = a.size - 1;
		RIO_ASSERT(last >= a.sparseToDenseTable[id.index], "Swapping with previous item");
		a.innerIdArrayData[a.sparseToDenseTable[id.index]] = a.innerIdArrayData[last];

		// Update tables
		uint16_t std = a.sparseToDenseTable[id.index];
		uint16_t dts = a.denseToSparseTable[last];
		a.sparseToDenseTable[dts] = std;
		a.denseToSparseTable[std] = dts;
		a.size--;
	}

	template <uint32_t MAX, typename T>
	inline T& get(IdArray<MAX, T>& a, const Id& id)
	{
		RIO_ASSERT(has(a, id), "IdArray does not have ID: %d,%d", id.id, id.index);
		return a.innerIdArrayData[a.sparseToDenseTable[id.index]];
	}

	template <uint32_t MAX, typename T>
	inline bool has(const IdArray<MAX, T>& a, Id id)
	{
		return id.index < MAX && a.sparseList[id.index].id == id.id;
	}

	template <uint32_t MAX, typename T>
	inline uint32_t getCount(const IdArray<MAX, T>& a)
	{
		return a.size;
	}

	template <uint32_t MAX, typename T>
	inline T* begin(IdArray<MAX, T>& a)
	{
		return a.innerIdArrayData;
	}

	template <uint32_t MAX, typename T>
	inline const T* begin(const IdArray<MAX, T>& a)
	{
		return a.innerIdArrayData;
	}

	template <uint32_t MAX, typename T>
	inline T* end(IdArray<MAX, T>& a)
	{
		return a.innerIdArrayData + a.size;
	}

	template <uint32_t MAX, typename T>
	inline const T* end(const IdArray<MAX, T>& a)
	{
		return a.innerIdArrayData + a.size;
	}
} // namespace IdArrayFn

template <uint32_t MAX, typename T>
inline IdArray<MAX, T>::IdArray()
	: freelist(Id::idInvalid)
	, nextId(0)
	, size(0)
{
	for (uint32_t i = 0; i < MAX; i++)
	{
		sparseList[i].id = Id::idInvalid;
	}
}

template <uint32_t MAX, typename T>
inline T& IdArray<MAX, T>::operator[](uint32_t i)
{
	RIO_ASSERT(i < size, "Index out of bounds");
	return innerIdArrayData[i];
}

template <uint32_t MAX, typename T>
inline const T& IdArray<MAX, T>::operator[](uint32_t i) const
{
	RIO_ASSERT(i < size, "Index out of bounds");
	return innerIdArrayData[i];
}

} // namespace Rio
