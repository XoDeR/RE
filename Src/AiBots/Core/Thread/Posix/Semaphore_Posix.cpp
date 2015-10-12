// Copyright (c) 2015 Volodymyr Syvochka
#include "Core/Thread/Semaphore.h"

#if RIO_PLATFORM_POSIX

namespace Rio
{
	Semaphore::Semaphore()
			: count(0)
	{
		int result = pthread_cond_init(&threadCond, NULL);
		RIO_ASSERT(result == 0, "pthread_cond_init: errno = %d", result);
		RIO_UNUSED(result);
	}

	Semaphore::~Semaphore()
	{
		int result = pthread_cond_destroy(&threadCond);
		RIO_ASSERT(result == 0, "pthread_cond_destroy: errno = %d", result);
		RIO_UNUSED(result);
	}

	void Semaphore::post(uint32_t count)
	{
		ScopedMutex sm(mutex);

		for (uint32_t i = 0; i < count; ++i)
		{
			int result = pthread_cond_signal(&threadCond);
			RIO_ASSERT(result == 0, "pthread_cond_signal: errno = %d", result);
			RIO_UNUSED(result);
		}

		this->count += count;
	}

	void Semaphore::wait()
	{
		ScopedMutex sm(mutex);
		while (count <= 0)
		{
			int result = pthread_cond_wait(&threadCond, &(mutex._mutex));
			RIO_ASSERT(result == 0, "pthread_cond_wait: errno = %d", result);
			RIO_UNUSED(result);
		}

		count--;
	}
} // namespace Rio

#endif  // RIO_PLATFORM_POSIX