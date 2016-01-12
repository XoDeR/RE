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

	// HashMap<T> is an unordered hash map/table for POD types
	// The key is a uint64_t so a hash function must return as a uint64_t
	// The HashMap can be a regular HashMap or a MultiHashMap through
	// Multi:: namespaced functions
	template<typename T>
	struct HashMap
	{
		HashMap(Allocator& a);
		struct Entry
		{
			uint64_t key;
			size_t next;
			T value;
		};

		Array<size_t> hashes;
		Array<Entry> hashData;

		ALLOCATOR_AWARE;
	};

	// The hash function stores its data in a "list-in-an-array" where indices are used instead of pointers.
	// When items are removed, the array-list is repacked to always keep it tightly ordered.
	namespace HashMapFn
	{
		template<typename T> bool has(const HashMap<T>& h, uint64_t key);
		template<typename T> const T& get(const HashMap<T>& h, uint64_t key, const T& defaultValue);
		template<typename T> void set(HashMap<T>& h, uint64_t key, const T& value);
		template<typename T> void remove(HashMap<T>& h, uint64_t key);
		template<typename T> void reserve(HashMap<T>& h, size_t size);
		template<typename T> void clear(HashMap<T>& h);
		// Returns a pointer to the first entry in the hash table, can be used to
		// efficiently iterate over the elements (in random order).
		template<typename T> const typename HashMap<T>::Entry* begin(const HashMap<T>& h);
		template<typename T> const typename HashMap<T>::Entry* end(const HashMap<T>& h);
	}

	namespace Hash32Fn
	{
		template<typename T> bool has(const HashMap<T>& h, uint32_t key);
		template<typename T> const T& get(const HashMap<T>& h, uint32_t key, const T& defaultValue);
		template<typename T> void set(HashMap<T>& h, uint32_t key, const T& value);
		template<typename T> void remove(HashMap<T>& h, uint32_t key);
		template<typename T> void reserve(HashMap<T>& h, size_t size);
		template<typename T> void clear(HashMap<T>& h);
		template<typename T> const typename HashMap<T>::Entry* begin(const HashMap<T>& h);
		template<typename T> const typename HashMap<T>::Entry* end(const HashMap<T>& h);
		template<typename T> size_t getCount(HashMap<T>& h);
	}

	// Functions to manipulate HashMap as a multi-hash
	namespace MultiHashMapFn
	{
		template<typename T> const typename HashMap<T>::Entry* findFirst(const HashMap<T> &h, uint64_t key);
		template<typename T> const typename HashMap<T>::Entry* findNext(const HashMap<T> &h, const typename HashMap<T>::Entry *e);
		// Returns the number of entries with the key.
		template<typename T> size_t getCountForKey(const HashMap<T>& h, uint64_t key);
		// Returns all the entries with the specified key.
		// Use a TempAllocator for the array to avoid allocating memory.
		template<typename T> void get(const HashMap<T>& h, uint64_t key, Array<T> &items);
		template<typename T> void insert(HashMap<T>& h, uint64_t key, const T &value);
		template<typename T> void remove(HashMap<T>& h, const typename HashMap<T>::Entry *e);
		template<typename T> void removeAll(HashMap<T>& h, uint64_t key);
	}

	namespace HashMapInternalFn
	{
		const size_t END_OF_LIST = (size_t)(-1);

		struct FindResult
		{
			size_t hashIndex;
			size_t dataPrev;
			size_t dataIndex;
		};

		template<typename T> size_t addEntry(HashMap<T>& h, uint64_t key)
		{
			typename HashMap<T>::Entry e;
			e.key = key;
			e.next = END_OF_LIST;
			size_t entryIndex = ArrayFn::getCount(h.hashData);
			ArrayFn::pushBack(h.hashData, e);
			return entryIndex;
		}

		template<typename T> FindResult find(const HashMap<T> &h, uint64_t key)
		{
			FindResult fr;
			fr.hashIndex = END_OF_LIST;
			fr.dataPrev = END_OF_LIST;
			fr.dataIndex = END_OF_LIST;

			if (ArrayFn::getCount(h.hashes) == 0)
			{
				return fr;
			}

			fr.hashIndex = key % ArrayFn::getCount(h.hashes);
			fr.dataIndex = h.hashes[fr.hashIndex];
			while (fr.dataIndex != END_OF_LIST)
			{
				if (h.hashData[fr.dataIndex].key == key)
				{
					return fr;
				}
				fr.dataPrev = fr.dataIndex;
				fr.dataIndex = h.hashData[fr.dataIndex].next;
			}
			return fr;
		}

		template<typename T> FindResult find(const HashMap<T> &h, const typename HashMap<T>::Entry *e)
		{
			FindResult fr;
			fr.hashIndex = END_OF_LIST;
			fr.dataPrev = END_OF_LIST;
			fr.dataIndex = END_OF_LIST;

			if (ArrayFn::getCount(h.hashes) == 0 || e == nullptr)
			{
				return fr;
			}

			fr.hashIndex = e->key % ArrayFn::getCount(h.hashes);
			fr.dataIndex = h.hashes[fr.hashIndex];
			while (fr.dataIndex != END_OF_LIST)
			{
				if (&h.hashData[fr.dataIndex] == e)
				{
					return fr;
				}
				fr.dataPrev = fr.dataIndex;
				fr.dataIndex = h.hashData[fr.dataIndex].next;
			}
			return fr;
		}

		template<typename T> void erase(HashMap<T>& h, const FindResult& fr)
		{
			// swap with the last element
			if (fr.dataPrev == END_OF_LIST)
			{
				h.hashes[fr.hashIndex] = h.hashData[fr.dataIndex].next;
			}
			else
			{
				h.hashData[fr.dataPrev].next = h.hashData[fr.dataIndex].next;
			}

			if (fr.dataIndex == ArrayFn::getCount(h.hashData) - 1)
			{
				ArrayFn::popBack(h.hashData);
				return;
			}

			h.hashData[fr.dataIndex] = h.hashData[ArrayFn::getCount(h.hashData) - 1];
			FindResult last = find(h, h.hashData[fr.dataIndex].key);

			if (last.dataPrev != END_OF_LIST)
			{
				h.hashData[last.dataPrev].next = fr.dataIndex;
			}
			else
			{
				h.hashes[last.hashIndex] = fr.dataIndex;
			}
		}

		template<typename T> size_t findOrFail(const HashMap<T>& h, uint64_t key)
		{
			return find(h, key).dataIndex;
		}

		template<typename T> size_t findOrMake(HashMap<T>& h, uint64_t key)
		{
			const FindResult fr = find(h, key);
			if (fr.dataIndex != END_OF_LIST)
			{
				return fr.dataIndex;
			}

			size_t i = addEntry(h, key);
			if (fr.dataPrev == END_OF_LIST)
			{
				h.hashes[fr.hashIndex] = i;
			}
			else
			{
				h.hashData[fr.dataPrev].next = i;
			}
			return i;
		}

		template<typename T> size_t make(HashMap<T>& h, uint64_t key)
		{
			const FindResult fr = find(h, key);
			const size_t i = addEntry(h, key);

			if (fr.dataPrev == END_OF_LIST)
			{
				h.hashes[fr.hashIndex] = i;
			}
			else
			{
				h.hashData[fr.dataPrev].next = i;
			}

			h.hashData[i].next = fr.dataIndex;
			return i;
		}

		template<typename T> void findAndErase(HashMap<T> &h, uint64_t key)
		{
			const FindResult fr = find(h, key);
			if (fr.dataIndex != END_OF_LIST)
			{
				erase(h, fr);
			}
		}

		template<typename T> void rehash(HashMap<T>& h, size_t newSize)
		{
			HashMap<T> nh(*h.hashes._allocator);
			ArrayFn::resize(nh.hashes, newSize);
			ArrayFn::reserve(nh.hashData, ArrayFn::getCount(h.hashData));
			for (size_t i = 0; i < newSize; ++i)
			{
				nh.hashes[i] = END_OF_LIST;
			}
			for (size_t i = 0; i < ArrayFn::getCount(h.hashData); ++i)
			{
				auto& e = h.data[i];
				MultiHashMapFn::insert(nh, e.key, e.value);
			}

			HashMap<T> empty(h.hashes.allocator);
			h.~HashMap<T>();
			memcpy(&h, &nh, sizeof(HashMap<T>));
			memcpy(&nh, &empty, sizeof(HashMap<T>));
		}

		template<typename T> bool isFull(const HashMap<T> &h)
		{
			// Ensure that there is enough space
			const float maximumLoadCoefficient = 0.7f;
			return ArrayFn::getCount(h.hashData) >= ArrayFn::getCount(h.hashes) * maximumLoadCoefficient;
		}

		template<typename T> void grow(HashMap<T> &h)
		{
			const uint32_t newSize = ArrayFn::getCount(h.hashData) * 2 + 10;
			rehash(h, newSize);
		}
	}

	namespace HashMapFn
	{
		template<typename T> bool has(const HashMap<T>& h, uint64_t key)
		{
			return HashMapInternalFn::findOrFail(h, key) != HashMapInternalFn::END_OF_LIST;
		}

		template<typename T> const T& get(const HashMap<T>& h, uint64_t key, const T& defaultValue)
		{
			const size_t i = HashMapInternalFn::findOrFail(h, key);
			return i == HashMapInternalFn::END_OF_LIST ? defaultValue : h.hashData[i].value;
		}

		template<typename T> void set(HashMap<T>& h, uint64_t key, const T &value)
		{
			if (ArrayFn::getCount(h.hashes) == 0)
			{
				HashMapInternalFn::grow(h);
			}

			const size_t i = HashMapInternalFn::findOrMake(h, key);
			h.hashData[i].value = value;
			if (HashMapInternalFn::isFull(h))
			{
				HashMapInternalFn::grow(h);
			}
		}

		template<typename T> void remove(HashMap<T>& h, uint64_t key)
		{
			HashMapInternalFn::findAndErase(h, key);
		}

		template<typename T> void reserve(HashMap<T>& h, uint32_t size)
		{
			HashMapInternalFn::rehash(h, size);
		}

		template<typename T> void clear(HashMap<T>& h)
		{
			ArrayFn::clear(h.hashData);
			ArrayFn::clear(h.hashes);
		}

		template<typename T> const typename HashMap<T>::Entry* begin(const HashMap<T>& h)
		{
			return ArrayFn::begin(h.hashData);
		}

		template<typename T> const typename HashMap<T>::Entry* end(const HashMap<T>& h)
		{
			return ArrayFn::end(h.hashData);
		}
	}

	namespace Hash32Fn
	{
		template<typename T> bool has(const HashMap<T>& h, uint32_t key)
		{
			return HashMapFn::has(h, static_cast<uint64_t>(key));
		}

		template<typename T> const T& get(const HashMap<T>& h, uint32_t key, const T& defaultValue)
		{
			return HashMapFn::get(h, static_cast<uint64_t>(key), defaultValue);
		}

		template<typename T> void set(HashMap<T>& h, uint32_t key, const T& value)
		{
			HashMapFn::set(h, static_cast<uint64_t>(key), value);
		}

		template<typename T> void remove(HashMap<T>& h, uint32_t key)
		{
			HashMapFn::remove(h, static_cast<uint64_t>(key));
		}

		template<typename T> void reserve(HashMap<T>& h, size_t size)
		{
			HashMapInternalFn::rehash(h, size);
		}

		template<typename T> void clear(HashMap<T>& h)
		{
			ArrayFn::clear(h.hashData);
			ArrayFn::clear(h.hashes);
		}

		template<typename T> const typename HashMap<T>::Entry* begin(const HashMap<T>& h)
		{
			return ArrayFn::begin(h.hashData);
		}

		template<typename T> const typename HashMap<T>::Entry* end(const HashMap<T>& h)
		{
			return ArrayFn::end(h.hashData);
		}

		template<typename T> size_t getCount(HashMap<T>& h)
		{
			return ArrayFn::getCount(h.hashData);
		}
	}

	namespace MultiHashMapFn
	{
		template<typename T> const typename HashMap<T>::Entry* findFirst(const HashMap<T>& h, uint64_t key)
		{
			const size_t i = HashMapInternalFn::findOrFail(h, key);
			return i == HashMapInternalFn::END_OF_LIST ? 0 : &h.hashData[i];
		}

		template<typename T> const typename HashMap<T>::Entry* findNext(const HashMap<T>& h, const typename HashMap<T>::Entry *e)
		{
			size_t i = e->next;
			while (i != HashMapInternalFn::END_OF_LIST)
			{
				if (h.hashData[i].key == e->key)
				{
					return &h.hashData[i];
				}
				i = h.hashData[i].next;
			}
			return 0;
		}

		template<typename T> size_t getCountForKey(const HashMap<T>& h, uint64_t key)
		{
			size_t i = 0;
			auto e = findFirst(h, key);
			while (e)
			{
				++i;
				e = findNext(h, e);
			}
			return i;
		}

		template<typename T> void get(const HashMap<T>& h, uint64_t key, Array<T> &items)
		{
			auto e = findFirst(h, key);
			while (e)
			{
				ArrayFn::pushBack(items, e->value);
				e = findNext(h, e);
			}
		}

		template<typename T> void insert(HashMap<T>& h, uint64_t key, const T& value)
		{
			if (ArrayFn::getCount(h.hashes) == 0)
			{
				HashMapInternalFn::grow(h);
			}

			const size_t i = HashMapInternalFn::make(h, key);
			h.hashData[i].value = value;
			if (HashMapInternalFn::isFull(h))
			{
				HashMapInternalFn::grow(h);
			}
		}

		template<typename T> void remove(HashMap<T>& h, const typename HashMap<T>::Entry* e)
		{
			const HashMapInternalFn::FindResult fr = HashMapInternalFn::find(h, e);
			if (fr.dataIndex != HashMapInternalFn::END_OF_LIST)
			{
				HashMapInternalFn::erase(h, fr);
			}
		}

		template<typename T> void removeAll(HashMap<T> &h, uint64_t key)
		{
			while (HashMapFn::has(h, key))
			{
				HashMapFn::remove(h, key);
			}
		}
	}


	template <typename T> HashMap<T>::HashMap(Allocator &a)
		: hashes(a)
		, hashData(a)
	{
	}

} // namespace Rio
