// Copyright (c) 2015 Volodymyr Syvochka
#include "Core/Thread/Thread.h"

#if RIO_PLATFORM_POSIX

namespace Rio
{

Thread::Thread()
	: threadHandle(0)
	, threadFunction(NULL)
	, threadData(NULL)
	, threadStackSize(0)
	, isRunning(false)
{
}

Thread::~Thread()
{
	if (isRunning)
	{
		stop();
	}
}

void Thread::start(ThreadFunction func, void* data, size_t stackSize)
{
	RIO_ASSERT(!isRunning, "Thread is already running");
	RIO_ASSERT(func != NULL, "Function must be != NULL");
	threadFunction = func;
	threadData = data;
	threadStackSize = stackSize;

	pthread_attr_t attr;
	int result = pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	RIO_ASSERT(result == 0, "pthread_attr_init: errno = %d", result);

	if (threadStackSize != 0)
	{
		result = pthread_attr_setstacksize(&attr, threadStackSize);
		RIO_ASSERT(result == 0, "pthread_attr_setstacksize: errno = %d", result);
	}

	result = pthread_create(&threadHandle, &attr, threadProc, this);
	RIO_ASSERT(result == 0, "pthread_create: errno = %d", result);

	// Free attr memory
	result = pthread_attr_destroy(&attr);
	RIO_ASSERT(result == 0, "pthread_attr_destroy: errno = %d", result);
	RIO_UNUSED(result);

	isRunning = true;
	semaphore.wait();
}

void Thread::stop()
{
	RIO_ASSERT(isRunning, "Thread is not running");

	int result = pthread_join(threadHandle, NULL);
	RIO_ASSERT(result == 0, "pthread_join: errno = %d", result);
	RIO_UNUSED(result);
	threadHandle = 0;

	isRunning = false;
}

} // namespace Rio

#endif // RIO_PLATFORM_POSIX