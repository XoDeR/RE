// Copyright (c) 2015 Volodymyr Syvochka
#pragma once

#include "Core/Base/Config.h"

#include "Core/Containers/Array.h"
#include <cstring> // memcpy

namespace Rio
{

	// HashMap<T> is an unordered hash map/table for POD types
	// The key is a uint64_t so a hash function must return as a uint64_t
	// The HashMap can be a regular HashMap or a MultiHashMap through
	// Multi:: namespaced functions
	template<typename T>
	struct Hash
	{
		Hash(Allocator& a);
		struct Entry
		{
			uint64_t key;
			size_t next;
			T value;
		};

		Array<size_t> hashes;
		Array<Entry> hashData;
	};

	// The hash function stores its data in a "list-in-an-array" where
	// indices are used instead of pointers.
	// When items are removed, the array-list is repacked to always keep
	// it tightly ordered.
	namespace HashFn
	{
		template<typename T> bool has(const Hash<T>& h, uint64_t key);
		template<typename T> const T& get(const Hash<T>& h, uint64_t key, const T& defaultValue);
		template<typename T> void set(Hash<T>& h, uint64_t key, const T& value);
		template<typename T> void remove(Hash<T>& h, uint64_t key);
		template<typename T> void reserve(Hash<T>& h, size_t size);
		template<typename T> void clear(Hash<T>& h);
		// Returns a pointer to the first entry in the hash table, can be used to
		// efficiently iterate over the elements (in random order).
		template<typename T> const typename Hash<T>::Entry* begin(const Hash<T>& h);
		template<typename T> const typename Hash<T>::Entry* end(const Hash<T>& h);
	}

	namespace Hash32Fn
	{
		template<typename T> bool has(const Hash<T>& h, uint32_t key);
		template<typename T> const T& get(const Hash<T>& h, uint32_t key, const T& defaultValue);
		template<typename T> void set(Hash<T>& h, uint32_t key, const T& value);
		template<typename T> void remove(Hash<T>& h, uint32_t key);
		template<typename T> void reserve(Hash<T>& h, size_t size);
		template<typename T> void clear(Hash<T>& h);
		template<typename T> const typename Hash<T>::Entry* begin(const Hash<T>& h);
		template<typename T> const typename Hash<T>::Entry* end(const Hash<T>& h);
		template<typename T> size_t getCount(Hash<T>& h);
	}

	// Functions to manipulate Hash as a multi-hash.
	namespace MultiHashFn
	{
		template<typename T> const typename Hash<T>::Entry* findFirst(const Hash<T> &h, uint64_t key);
		template<typename T> const typename Hash<T>::Entry* findNext(const Hash<T> &h, const typename Hash<T>::Entry *e);

		// Returns the number of entries with the key.
		template<typename T> size_t getCountForKey(const Hash<T>& h, uint64_t key);

		// Returns all the entries with the specified key.
		// Use a TempAllocator for the array to avoid allocating memory.
		template<typename T> void get(const Hash<T>& h, uint64_t key, Array<T> &items);

		// Inserts the value as an additional value for the key.
		template<typename T> void insert(Hash<T>& h, uint64_t key, const T &value);

		// Removes the specified entry.
		template<typename T> void remove(Hash<T>& h, const typename Hash<T>::Entry *e);

		// Removes all entries with the specified key.
		template<typename T> void removeAll(Hash<T>& h, uint64_t key);
	}

	namespace HashInternalFn
	{
		const size_t END_OF_LIST = (size_t)(-1);

		struct FindResult
		{
			size_t hashIndex;
			size_t dataPrev;
			size_t dataIndex;
		};

		template<typename T> size_t addEntry(Hash<T>& h, uint64_t key)
		{
			typename Hash<T>::Entry e;
			e.key = key;
			e.next = END_OF_LIST;
			size_t entryIndex = ArrayFn::getCount(h.hashData);
			ArrayFn::pushBack(h.hashData, e);
			return entryIndex;
		}

		template<typename T> FindResult find(const Hash<T> &h, uint64_t key)
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

		template<typename T> FindResult find(const Hash<T> &h, const typename Hash<T>::Entry *e)
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

		template<typename T> void erase(Hash<T>& h, const FindResult& fr)
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

		template<typename T> size_t findOrFail(const Hash<T>& h, uint64_t key)
		{
			return find(h, key).dataIndex;
		}

		template<typename T> size_t findOrMake(Hash<T>& h, uint64_t key)
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

		template<typename T> size_t make(Hash<T>& h, uint64_t key)
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

		template<typename T> void findAndErase(Hash<T> &h, uint64_t key)
		{
			const FindResult fr = find(h, key);
			if (fr.dataIndex != END_OF_LIST)
			{
				erase(h, fr);
			}
		}

		template<typename T> void rehash(Hash<T>& h, size_t newSize)
		{
			Hash<T> nh(*h.hashes._allocator);
			ArrayFn::resize(nh.hashes, newSize);
			ArrayFn::reserve(nh.hashData, ArrayFn::getCount(h.hashData));
			for (size_t i = 0; i < newSize; ++i)
			{
				nh.hashes[i] = END_OF_LIST;
			}
			for (size_t i = 0; i < ArrayFn::getCount(h.hashData); ++i)
			{
				auto& e = h.data[i];
				MultiHashFn::insert(nh, e.key, e.value);
			}

			Hash<T> empty(h.hashes.allocator);
			h.~Hash<T>();
			memcpy(&h, &nh, sizeof(Hash<T>));
			memcpy(&nh, &empty, sizeof(Hash<T>));
		}

		template<typename T> bool isFull(const Hash<T> &h)
		{
			// Ensure that there is enough space
			const float maximumLoadCoefficient = 0.7f;
			return ArrayFn::getCount(h.hashData) >= ArrayFn::getCount(h.hashes) * maximumLoadCoefficient;
		}

		template<typename T> void grow(Hash<T> &h)
		{
			const uint32_t newSize = ArrayFn::getCount(h.hashData) * 2 + 10;
			rehash(h, newSize);
		}
	}

	namespace HashFn
	{
		template<typename T> bool has(const Hash<T>& h, uint64_t key)
		{
			return HashInternalFn::findOrFail(h, key) != HashInternalFn::END_OF_LIST;
		}

		template<typename T> const T& get(const Hash<T>& h, uint64_t key, const T& defaultValue)
		{
			const size_t i = HashInternalFn::findOrFail(h, key);
			return i == HashInternalFn::END_OF_LIST ? defaultValue : h.hashData[i].value;
		}

		template<typename T> void set(Hash<T>& h, uint64_t key, const T &value)
		{
			if (ArrayFn::getCount(h.hashes) == 0)
			{
				HashInternalFn::grow(h);
			}

			const size_t i = HashInternalFn::findOrMake(h, key);
			h.hashData[i].value = value;
			if (HashInternalFn::isFull(h))
			{
				HashInternalFn::grow(h);
			}
		}

		template<typename T> void remove(Hash<T>& h, uint64_t key)
		{
			HashInternalFn::findAndErase(h, key);
		}

		template<typename T> void reserve(Hash<T>& h, uint32_t size)
		{
			HashInternalFn::rehash(h, size);
		}

		template<typename T> void clear(Hash<T>& h)
		{
			ArrayFn::clear(h.hashData);
			ArrayFn::clear(h.hashes);
		}

		template<typename T> const typename Hash<T>::Entry* begin(const Hash<T>& h)
		{
			return ArrayFn::begin(h.hashData);
		}

		template<typename T> const typename Hash<T>::Entry* end(const Hash<T>& h)
		{
			return ArrayFn::end(h.hashData);
		}
	}

	namespace Hash32Fn
	{
		template<typename T> bool has(const Hash<T>& h, uint32_t key)
		{
			return HashFn::has(h, static_cast<uint64_t>(key));
		}

		template<typename T> const T& get(const Hash<T>& h, uint32_t key, const T& defaultValue)
		{
			return HashFn::get(h, static_cast<uint64_t>(key), defaultValue);
		}

		template<typename T> void set(Hash<T>& h, uint32_t key, const T& value)
		{
			HashFn::set(h, static_cast<uint64_t>(key), value);
		}

		template<typename T> void remove(Hash<T>& h, uint32_t key)
		{
			HashFn::remove(h, static_cast<uint64_t>(key));
		}

		template<typename T> void reserve(Hash<T>& h, size_t size)
		{
			HashInternalFn::rehash(h, size);
		}

		template<typename T> void clear(Hash<T>& h)
		{
			ArrayFn::clear(h.hashData);
			ArrayFn::clear(h.hashes);
		}

		template<typename T> const typename Hash<T>::Entry* begin(const Hash<T>& h)
		{
			return ArrayFn::begin(h.hashData);
		}

		template<typename T> const typename Hash<T>::Entry* end(const Hash<T>& h)
		{
			return ArrayFn::end(h.hashData);
		}

		template<typename T> size_t getCount(Hash<T>& h)
		{
			return ArrayFn::getCount(h.hashData);
		}
	}

	namespace MultiHashFn
	{
		template<typename T> const typename Hash<T>::Entry* findFirst(const Hash<T>& h, uint64_t key)
		{
			const size_t i = HashInternalFn::findOrFail(h, key);
			return i == HashInternalFn::END_OF_LIST ? 0 : &h.hashData[i];
		}

		template<typename T> const typename Hash<T>::Entry* findNext(const Hash<T>& h, const typename Hash<T>::Entry *e)
		{
			size_t i = e->next;
			while (i != HashInternalFn::END_OF_LIST)
			{
				if (h.hashData[i].key == e->key)
				{
					return &h.hashData[i];
				}
				i = h.hashData[i].next;
			}
			return 0;
		}

		template<typename T> size_t getCountForKey(const Hash<T>& h, uint64_t key)
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

		template<typename T> void get(const Hash<T>& h, uint64_t key, Array<T> &items)
		{
			auto e = findFirst(h, key);
			while (e)
			{
				ArrayFn::pushBack(items, e->value);
				e = findNext(h, e);
			}
		}

		template<typename T> void insert(Hash<T>& h, uint64_t key, const T& value)
		{
			if (ArrayFn::getCount(h.hashes) == 0)
			{
				HashInternalFn::grow(h);
			}

			const size_t i = HashInternalFn::make(h, key);
			h.hashData[i].value = value;
			if (HashInternalFn::isFull(h))
			{
				HashInternalFn::grow(h);
			}
		}

		template<typename T> void remove(Hash<T>& h, const typename Hash<T>::Entry* e)
		{
			const HashInternalFn::FindResult fr = HashInternalFn::find(h, e);
			if (fr.dataIndex != HashInternalFn::END_OF_LIST)
			{
				HashInternalFn::erase(h, fr);
			}
		}

		template<typename T> void removeAll(Hash<T> &h, uint64_t key)
		{
			while (HashFn::has(h, key))
			{
				HashFn::remove(h, key);
			}
		}
	}


	template <typename T> Hash<T>::Hash(Allocator &a)
		: hashes(a)
		, hashData(a)
	{
	}

} // namespace Rio
