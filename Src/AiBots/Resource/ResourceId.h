// Copyright (c) 2015 Volodymyr Syvochka
#pragma once

#include "Core/Base/Config.h"
#include "Core/Base/Types.h"
#include "Core/Base/Murmur.h"
#include "Core/Strings/StringUtils.h"
#include "Core/Strings/StringId64.h"

namespace Rio
{

	struct ResourceId
	{
		ResourceId()
			: type(uint64_t(0))
			, name(uint64_t(0))
		{
		}

		ResourceId(StringId64 type, StringId64 name)
			: type(type)
			, name(name)
		{
		}

		ResourceId(const char* type, const char* name)
			: type(murmur64(type, strLen(type), 0))
			, name(murmur64(name, strLen(name), SEED))
		{
		}

		const char* toString(char out[64])
		{
			snPrintf(out, 64, "%.16" PRIx64 "-%.16" PRIx64, type.getId(), name.getId());
			return out;
		}

		bool operator==(const ResourceId& a) const
		{
			return type == a.type && name == a.name;
		}

		bool operator<(const ResourceId& a) const
		{
			return type < a.type || (type == a.type && name < a.name);
		}

		static const uint64_t SEED = 0;

		StringId64 type;
		StringId64 name;
	};

} // namespace Rio
