// Copyright (c) 2015 Volodymyr Syvochka
#pragma once

#include "Core/Base/Config.h"
#include "Core/Base/Types.h"

namespace Rio 
{
uint32_t murmur32(const void* key, size_t len, uint32_t seed = 0);
uint64_t murmur64(const void* key, int len, uint64_t seed = 0);
} // namespace Rio
