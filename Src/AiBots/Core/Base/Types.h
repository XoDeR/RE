// Copyright (c) 2015 Volodymyr Syvochka
#pragma once

#include "Core/Base/Config.h"

#include <cstddef>
#include <stdint.h>
#include <stdio.h>

#include <type_traits>
#include <memory>

#if RIO_COMPILER_MSVC
	#define RIO_ALIGNOF(x) __alignof(x)
	#define _ALLOW_KEYWORD_MACROS
#elif RIO_COMPILER_GCC
	#define RIO_ALIGNOF(x) __alignof__(x)
#else
	#error "Compiler not supported"
#endif

#define RIO_UNUSED(x) do { (void)(x); } while (0)

namespace Rio
{
	static_assert(sizeof(int64_t) == sizeof(size_t), "size_t size is not 64 bits");
} // namespace Rio


