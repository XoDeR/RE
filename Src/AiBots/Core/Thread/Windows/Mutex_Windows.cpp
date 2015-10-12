// Copyright (c) 2015 Volodymyr Syvochka

#include "Core/Thread/Mutex.h"

#if RIO_PLATFORM_WINDOWS

namespace Rio
{
	Mutex::Mutex()
	{
		InitializeCriticalSection(&criticalSection);
	}

	Mutex::~Mutex()
	{
		DeleteCriticalSection(&criticalSection);
	}

	void Mutex::lock()
	{
		EnterCriticalSection(&criticalSection);
	}

	void Mutex::unlock()
	{
		LeaveCriticalSection(&criticalSection);
	}
} // namespace Rio

#endif // RIO_PLATFORM_WINDOWS