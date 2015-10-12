// Copyright (c) 2015 Volodymyr Syvochka
#pragma once

#include "Core/Base/Config.h"
#include "Core/Debug/Error.h"
#include "Core/Base/Types.h"
#include "Semaphore.h"

#if RIO_PLATFORM_POSIX
	#include <pthread.h>
#elif RIO_PLATFORM_WINDOWS
	#include "Core/Os/Windows/Headers_Windows.h"
	#include <process.h>
#endif

namespace Rio 
{
	typedef int32_t(*ThreadFunction)(void*);

struct Thread
{
	Thread();
	~Thread();
	void start(ThreadFunction func, void* data = NULL, size_t stackSize = 0);
	void stop();
	bool getIsRunning()
	{
		return isRunning;
	}
private:
	int32_t run()
	{
		semaphore.post();
		return threadFunction(threadData);
	}

#if RIO_PLATFORM_POSIX
	static void* threadProc(void* arg)
	{
		static int32_t result = -1;
		result = ((Thread*)arg)->run();
		return (void*)&result;
	}
#elif RIO_PLATFORM_WINDOWS
	static DWORD WINAPI threadProc(void* arg)
	{
		Thread* thread = reinterpret_cast<Thread*>(arg);
		int32_t result = thread->run();
		return result;
	}
#endif

private:
#if RIO_PLATFORM_POSIX
	pthread_t threadHandle;
#elif RIO_PLATFORM_WINDOWS
	HANDLE threadHandle;
#endif
	ThreadFunction threadFunction;
	void* threadData;
	Semaphore semaphore;
	size_t threadStackSize;
	bool isRunning;
private:
	// Disable copying
	Thread(const Thread&);
	Thread& operator=(const Thread&);
};

} // namespace Rio
