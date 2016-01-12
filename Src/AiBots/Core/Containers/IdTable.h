// Copyright (c) 2015 Volodymyr Syvochka
#pragma once

#include "Core/Base/Config.h"

#include "Core/Debug/Error.h"
#include "Core/Base/Types.h"
#include "Core/Memory/Allocator.h"
#include "Core/Base/Id.h"

namespace Rio
{
	// Table of Ids.
	template <uint32_t MAX>
	struct IdTable
	{
		// Table for [MAX - 1] unique Ids
		IdTable();
		// The index of the first unused id.
		uint16_t freelist;
		// Next available unique id.
		uint16_t nextId;
		uint16_t size;
		// Table of ids.
		// The last valid id is reserved and cannot be used to
		// refer to Ids from the outside.
		Id idTable[MAX];
	};

	// Functions to manipulate IdTable.
	namespace IdTableFn
	{
		// Creates a new Id in the table a and returns its value.
		template <uint32_t MAX> Id create(IdTable<MAX>& a);
		// Destroys the id in the table a.
		template <uint32_t MAX> void destroy(IdTable<MAX>& a, Id id);
		// Returns whether the table a has the given id.
		template <uint32_t MAX> bool has(const IdTable<MAX>& a, Id id);
		// Returns the number of ids in the table a.
		template <uint32_t MAX> uint16_t getCount(const IdTable<MAX>& a);
		template <uint32_t MAX> const Id* begin(const IdTable<MAX>& a);
		template <uint32_t MAX> const Id* end(const IdTable<MAX>& a);
	} // namespace IdTableFn

	namespace IdTableFn
	{
		template <uint32_t MAX>
		inline Id create(IdTable<MAX>& a)
		{
			// Obtain a new id
			Id id;
			id.id = a.nextId++;

			// Recycle slot if there are any
			if (a.freelist != Id::idInvalid)
			{
				id.index = a.freelist;
				a.freelist = a.idTable[a.freelist].index;
			}
			else
			{
				id.index = a.size;
			}

			a.idTable[id.index] = id;
			a.size++;

			return id;
		}

		template <uint32_t MAX>
		inline void destroy(IdTable<MAX>& a, Id id)
		{
			RIO_ASSERT(has(a, id), "IdTable does not have ID: %d,%d", id.id, id.index);

			a.idTable[id.index].id = Id::idInvalid;
			a.idTable[id.index].index = a.freelist;
			a.freelist = id.index;
			a.size--;
		}

		template <uint32_t MAX>
		inline bool has(const IdTable<MAX>& a, Id id)
		{
			return id.index < MAX && a.idTable[id.index].id == id.id;
		}

		template <uint32_t MAX>
		inline uint16_t getCount(const IdTable<MAX>& a)
		{
			return a.size;
		}

		template <uint32_t MAX>
		inline const Id* begin(const IdTable<MAX>& a)
		{
			return a.idTable;
		}

		template <uint32_t MAX>
		inline const Id* end(const IdTable<MAX>& a)
		{
			return a.idTable + MAX;
		}
	} // namespace IdTableFn

	template <uint32_t MAX>
	inline IdTable<MAX>::IdTable()
		: freelist(Id::idInvalid)
		, nextId(0)
		, size(0)
	{
		for (uint32_t i = 0; i < MAX; i++)
		{
			idTable[i].id = Id::idInvalid;
		}
	}

} // namespace Rio
