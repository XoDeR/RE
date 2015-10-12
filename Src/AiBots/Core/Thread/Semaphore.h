// Copyright (c) 2015 Volodymyr Syvochka
#pragma once

#include "Core/Base/Config.h"

#include "Core/Debug/Error.h"
#include "Core/Thread/Mutex.h"

#if RIO_PLATFORM_POSIX
	#include <pthread.h>
#elif RIO_PLATFORM_WINDOWS
	#include "Core/Os/Windows/Headers_Windows.h"
	#include <limits.h>
#endif

namespace Rio 
{

class Semaphore
{
public:
	Semaphore();
	~Semaphore();
	void post(uint32_t count = 1);
	void wait();
private:
#if RIO_PLATFORM_POSIX
	Mutex mutex;
	pthread_cond_t threadCond;
	int32_t count;
#elif RIO_PLATFORM_WINDOWS
	HANDLE semaphoreHandle;
#endif
private:
	// Disable copying
	Semaphore(const Semaphore& s);
	Semaphore& operator=(const Semaphore& s);
};

} // namespace Rio
