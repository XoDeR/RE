// Copyright (c) 2015 Volodymyr Syvochka
#pragma once

#include "Core/Base/Config.h"
#include "Core/Base/Types.h"

#if RIO_PLATFORM_POSIX
	#include <pthread.h>
#elif RIO_PLATFORM_WINDOWS
	#include "Core/Os/Windows/Headers_Windows.h"
#endif

namespace Rio 
{
// Why not use std::mutex and std::lock_guard
// CriticalSection should be faster than std::mutex for Windows
// For POSIX std::mutex uses PThreads internally
class Mutex
{
public:
	Mutex();
	~Mutex();
	void lock();
	void unlock();
private:
#if RIO_PLATFORM_POSIX
	pthread_mutex_t mutex;
	pthread_mutexattr_t mutexAttr;
#elif RIO_PLATFORM_WINDOWS
	CRITICAL_SECTION criticalSection;
#endif
private:
	// Disable copying.
	Mutex(const Mutex&);
	Mutex& operator=(const Mutex&);
};
} // namespace Rio
