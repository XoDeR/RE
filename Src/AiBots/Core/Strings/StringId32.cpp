// Copyright (c) 2015 Volodymyr Syvochka
#include "Core/Strings/StringId32.h"
#include "Core/Base/Murmur.h"
#include "Core/Strings/StringUtils.h"

namespace Rio
{

StringId32::StringId32(const char* str)
	: id(murmur32(str, strLen(str)))
{
}

StringId32::StringId32(const char* str, uint32_t len)
	: id(murmur32(str, len))
{
}

} // namespace Rio