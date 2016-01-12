// Copyright (c) 2015 Volodymyr Syvochka
#include "Core/Strings/StringId64.h"
#include "Core/Base/Murmur.h"
#include "Core/Strings/StringUtils.h"
#include <inttypes.h> // PRIx64

namespace Rio
{
	StringId64::StringId64(const char* str)
		: id(murmur64(str, strLen32(str)))
	{
	}

	StringId64::StringId64(const char* str, uint32_t len)
		: id(murmur64(str, len))
	{
	}

	const char* StringId64::toString(char* buf)
	{
		snPrintf(buf, STRING_LENGTH, "%.16" PRIx64, id);
		return buf;
	}
} // namespace Rio