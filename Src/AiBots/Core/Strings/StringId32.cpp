// Copyright (c) 2015 Volodymyr Syvochka
#include "Core/Strings/StringId32.h"
#include "Core/Base/Murmur.h"
#include "Core/Strings/StringUtils.h"
#include <inttypes.h> // PRIx64

namespace Rio
{

	StringId32::StringId32(const char* str)
		: id(murmur32(str, strLen32(str)))
	{
	}

	StringId32::StringId32(const char* str, uint32_t len)
		: id(murmur32(str, len))
	{
	}

	const char* StringId32::toString(char* buf)
	{
		snPrintf(buf, STRING_LENGTH, "%.8x", id);
		return buf;
	}

} // namespace Rio