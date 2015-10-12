// Copyright (c) 2015 Volodymyr Syvochka
#include "Core/Thread/AtomicInt.h"

#if RIO_PLATFORM_WINDOWS
namespace Rio
{
	AtomicInt::AtomicInt(int val)
	{
		store(val);
	}

	int AtomicInt::load() const
	{
		InterlockedExchangeAdd(&valAtomic, (int32_t)0);
		return valAtomic;
	}

	void AtomicInt::store(int val)
	{
		InterlockedExchange(&valAtomic, val);
	}
} // namespace Rio
#endif RIO_PLATFORM_WINDOWS