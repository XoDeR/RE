// Copyright (c) 2015 Volodymyr Syvochka
#include "Core/Thread/AtomicInt.h"

#if RIO_PLATFORM_POSIX && RIO_COMPILER_GCC
namespace Rio
{
	AtomicInt::AtomicInt(int val)
	{
		store(val);
	}

	int AtomicInt::load() const
	{
		__sync_fetch_and_add(&valAtomic, 0);
		return valAtomic;
	}

	void AtomicInt::store(int val)
	{
		__sync_lock_test_and_set(&valAtomic, val);
	}
} // namespace Rio
#endif // RIO_PLATFORM_POSIX && RIO_COMPILER_GCC
