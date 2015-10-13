// Copyright (c) 2015 Volodymyr Syvochka
#include "Core/Strings/StringId64.h"
#include "Core/Base/Murmur.h"
#include "Core/Strings/StringUtils.h"

namespace Rio
{

StringId64::StringId64(const char* str)
	: id(murmur64(str, strlen(str)))
{
}

StringId64::StringId64(const char* str, uint32_t len)
	: id(murmur64(str, len))
{
}

} // namespace Rio