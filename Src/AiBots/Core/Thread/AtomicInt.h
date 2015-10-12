// Copyright (c) 2015 Volodymyr Syvochka
#pragma once

#include "Core/Base/Config.h"

#if RIO_PLATFORM_WINDOWS
	#include "Core/Base/Types.h"
	#include "Core/Os/Windows/Headers_Windows.h"
#endif

namespace Rio 
{

class AtomicInt
{
public:
	AtomicInt(int val);
	int load() const;
	void store(int val);
private:
#if RIO_PLATFORM_POSIX && RIO_COMPILER_GCC
	mutable int valAtomic;
#elif RIO_PLATFORM_WINDOWS
	mutable LONG valAtomic;
#endif
};

} // namespace Rio
