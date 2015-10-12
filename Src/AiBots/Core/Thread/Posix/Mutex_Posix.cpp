// Copyright (c) 2015 Volodymyr Syvochka
#include "Core/Thread/Mutex.h"
#include "Core/Debug/Error.h"

#if RIO_PLATFORM_POSIX

namespace Rio
{
	Mutex::Mutex()
	{
		int result = pthread_mutexattr_init(&mutexAttr);
		RIO_ASSERT(result == 0, "pthread_mutexattr_init: errno = %d", result);
		result = pthread_mutexattr_settype(&mutexAttr, PTHREAD_MUTEX_ERRORCHECK);
		RIO_ASSERT(result == 0, "pthread_mutexattr_settype: errno = %d", result);
		result = pthread_mutex_init(&mutex, &mutexAttr);
		RIO_ASSERT(result == 0, "pthread_mutex_init: errno = %d", result);
		RIO_UNUSED(result);
	}

	Mutex::~Mutex()
	{
		int result = pthread_mutex_destroy(&mutex);
		RIO_ASSERT(result == 0, "pthread_mutex_destroy: errno = %d", result);
		result = pthread_mutexattr_destroy(&mutexAttr);
		RIO_ASSERT(result == 0, "pthread_mutexattr_destroy: errno = %d", result);
		RIO_UNUSED(result);
	}

	void Mutex::lock()
	{
		int result = pthread_mutex_lock(&mutex);
		RIO_ASSERT(result == 0, "pthread_mutex_lock: errno = %d", result);
		RIO_UNUSED(result);
	}

	void Mutex::unlock()
	{
		int result = pthread_mutex_unlock(&mutex);
		RIO_ASSERT(result == 0, "pthread_mutex_unlock: errno = %d", result);
		RIO_UNUSED(result);
	}
} // namespace Rio

#endif RIO_PLATFORM_POSIX