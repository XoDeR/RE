// Copyright (c) 2015 Volodymyr Syvochka
#pragma once

#include "Core/Base/Config.h"

#include "Core/Base/Types.h"

#include "Core/Debug/Error.h" // RIO_ASSERT

#include "Core/Memory/Memory.h"
#include "Core/Memory/Allocator.h"

#include "Core/Containers/Vector.h"
#include "Core/Containers/Pair.h"
#include "Core/Containers/Functional.h"

#include <cstring> // memcpy
#include <algorithm>

namespace Rio
{
	// Sorted map from key to POD items.
	// Items are not automatically sorted, you need to call SortMapFn::sort().
	template <typename TKey, typename TValue, class Compare = less<TKey> >
	struct SortMap
	{
		SortMap(Allocator& a);
		struct Entry
		{
			Entry(Allocator& a)
				: pair(a)
			{
			}

			PAIR(TKey, TValue) pair;

			ALLOCATOR_AWARE;
		};

		Vector<Entry> innerSortMapData;
#if RIO_DEBUG
		bool isMapSorted = true;
#endif // RIO_DEBUG
		ALLOCATOR_AWARE;
	};

	namespace SortMapFn
	{
		// Returns the number of items in the map
		template <typename TKey, typename TValue, typename Compare> uint32_t getCount(const SortMap<TKey, TValue, Compare>& m);
		// Returns whether the key exists in the map
		template <typename TKey, typename TValue, typename Compare> bool has(const SortMap<TKey, TValue, Compare>& m, const TKey& key);
		// Returns the value for the given key or default if
		// the key does not exist in the map
		template <typename TKey, typename TValue, typename Compare> const TValue& get(const SortMap<TKey, TValue, Compare>& m, const TKey& key, const TValue& defaultValue);
		// Returns the value for the given key or default if
		// the key does not exist in the map
		template <typename TKey, typename TValue, typename Compare> TValue& get(SortMap<TKey, TValue, Compare>& m, const TKey& key, const TValue& defaultValue);
		// Sorts the keys in the map
		template <typename TKey, typename TValue, typename Compare> void sort(SortMap<TKey, TValue, Compare>& m);
		template <typename TKey, typename TValue, typename Compare> void set(SortMap<TKey, TValue, Compare>& m, const TKey& key, const TValue& val);
		// Removes the key from the map if it exists
		template <typename TKey, typename TValue, typename Compare> void remove(SortMap<TKey, TValue, Compare>& m, const TKey& key);
		// Removes all the items in the map
		template <typename TKey, typename TValue, typename Compare> void clear(SortMap<TKey, TValue, Compare>& m);
		// Returns a pointer to the first item in the map, can be used to
		// efficiently iterate over the elements.
		template <typename TKey, typename TValue, typename Compare> const typename SortMap<TKey, TValue, Compare>::Entry* begin(const SortMap<TKey, TValue, Compare>& m);
		template <typename TKey, typename TValue, typename Compare> const typename SortMap<TKey, TValue, Compare>::Entry* end(const SortMap<TKey, TValue, Compare>& m);
	} // namespace SortMapFn

	namespace SortMapInternalFn
	{
		const uint32_t END_OF_LIST = 0xFFFFFFFFu;

		struct FindResult
		{
			uint32_t item_i;
		};

		template <typename TKey, typename TValue, typename Compare>
		struct CompareEntry
		{
			bool operator()(const typename SortMap<TKey, TValue, Compare>::Entry& a,
				const typename SortMap<TKey, TValue, Compare>::Entry& b) const
			{
				return comp(a.pair.first, b.pair.second);
			}

			bool operator()(const typename SortMap<TKey, TValue, Compare>::Entry& a,
				const TKey& key) const
			{
				return comp(a.pair.first, key);
			}

			Compare comp;
		};

		template <typename TKey, typename TValue, typename Compare>
		inline FindResult find(const SortMap<TKey, TValue, Compare>& m, const TKey& key)
		{
			RIO_ASSERT(m.isMapSorted, "Map not sorted");

			FindResult result;
			result.item_i = END_OF_LIST;

			const typename SortMap<TKey, TValue, Compare>::Entry* first =
				std::lower_bound(VectorFn::begin(m.innerSortMapData), VectorFn::end(m.innerSortMapData), key,
				SortMapInternalFn::CompareEntry<TKey, TValue, Compare>());

			if (first != VectorFn::end(m.innerSortMapData) && !(key < first->pair.first))
			{
				result.item_i = first - VectorFn::begin(m.innerSortMapData);
			}

			return result;
		}
	} // namespace SortMapInternalFn

	namespace SortMapFn
	{
		template <typename TKey, typename TValue, typename Compare>
		inline uint32_t getCount(const SortMap<TKey, TValue, Compare>& m)
		{
			return VectorFn::getCount(m.innerSortMapData);
		}

		template <typename TKey, typename TValue, typename Compare>
		inline bool has(const SortMap<TKey, TValue, Compare>& m, const TKey& key)
		{
			return SortMapInternalFn::find(m, key).item_i != SortMapInternalFn::END_OF_LIST;
		}

		template <typename TKey, typename TValue, typename Compare>
		const TValue& get(const SortMap<TKey, TValue, Compare>& m, const TKey& key, const TValue& defaultValue)
		{
			SortMapInternalFn::FindResult result = SortMapInternalFn::find(m, key);

			if (result.item_i == SortMapInternalFn::END_OF_LIST)
			{
				return defaultValue;
			}

			return m.innerSortMapData[result.item_i].pair.second;
		}

		template <typename TKey, typename TValue, typename Compare>
		TValue& get(SortMap<TKey, TValue, Compare>& m, const TKey& key, const TValue& defaultValue)
		{
			return const_cast<TValue&>(get(static_cast<const SortMap<TKey, TValue, Compare>&>(m), key, defaultValue));
		}

		template <typename TKey, typename TValue, typename Compare>
		inline void sort(SortMap<TKey, TValue, Compare>& m)
		{
			std::sort(VectorFn::begin(m.innerSortMapData), VectorFn::end(m.innerSortMapData),
				SortMapInternalFn::CompareEntry<TKey, TValue, Compare>());
#if RIO_DEBUG
			m.isMapSorted = true;
#endif // RIO_DEBUG
		}

		template <typename TKey, typename TValue, typename Compare>
		inline void set(SortMap<TKey, TValue, Compare>& m, const TKey& key, const TValue& val)
		{
			SortMapInternalFn::FindResult result = SortMapInternalFn::find(m, key);

			if (result.item_i == SortMapInternalFn::END_OF_LIST)
			{
				typename SortMap<TKey, TValue, Compare>::Entry e(*m.innerSortMapData.allocator);
				e.pair.first = key;
				e.pair.second = val;
				VectorFn::pushBack(m.innerSortMapData, e);
			}
			else
			{
				m.innerSortMapData[result.item_i].pair.second = val;
			}
#if RIO_DEBUG
			m.isMapSorted = false;
#endif // RIO_DEBUG
		}

		template <typename TKey, typename TValue, typename Compare>
		inline void remove(SortMap<TKey, TValue, Compare>& m, const TKey& key)
		{
			SortMapInternalFn::FindResult result = SortMapInternalFn::find(m, key);

			if (result.item_i == SortMapInternalFn::END_OF_LIST)
				return;

			if (VectorFn::getCount(m.innerSortMapData))
			{
				m.innerSortMapData[result.item_i] = m.innerSortMapData[VectorFn::getCount(m.innerSortMapData) - 1];
				VectorFn::popBack(m.innerSortMapData);
			}
#if RIO_DEBUG
			m.isMapSorted = false;
#endif // RIO_DEBUG
		}

		template <typename TKey, typename TValue, typename Compare>
		inline void clear(SortMap<TKey, TValue, Compare>& m)
		{
			VectorFn::clear(m.innerSortMapData);
#if RIO_DEBUG
			m.isMapSorted = true;
#endif // RIO_DEBUG
		}

		template <typename TKey, typename TValue, typename Compare>
		inline const typename SortMap<TKey, TValue, Compare>::Entry* begin(const SortMap<TKey, TValue, Compare>& m)
		{
			return VectorFn::begin(m.innerSortMapData);
		}

		template <typename TKey, typename TValue, typename Compare>
		inline const typename SortMap<TKey, TValue, Compare>::Entry* end(const SortMap<TKey, TValue, Compare>& m)
		{
			return VectorFn::end(m.innerSortMapData);
		}
	} // namespace SortMapFn

	template <typename TKey, typename TValue, typename Compare>
	inline SortMap<TKey, TValue, Compare>::SortMap(Allocator& a)
		: innerSortMapData(a)
	{
	}

} // namespace Rio
