// Copyright (c) 2015 Volodymyr Syvochka
#include "Core/Thread/Semaphore.h"

#if RIO_PLATFORM_WINDOWS

namespace Rio
{
	Semaphore::Semaphore()
		: semaphoreHandle(INVALID_HANDLE_VALUE)
	{
		semaphoreHandle = CreateSemaphore(NULL, 0, LONG_MAX, NULL);
		RIO_ASSERT(semaphoreHandle != NULL, "CreateSemaphore: GetLastError = %d", GetLastError());
		RIO_UNUSED(semaphoreHandle);
	}

	Semaphore::~Semaphore()
	{
		BOOL err = CloseHandle(semaphoreHandle);
		RIO_ASSERT(err != 0, "CloseHandle: GetLastError = %d", GetLastError());
		RIO_UNUSED(err);
	}

	void Semaphore::post(uint32_t count)
	{
		BOOL err = ReleaseSemaphore(semaphoreHandle, count, NULL);
		RIO_ASSERT(err != 0, "ReleaseSemaphore: GetLastError = %d", GetLastError());
		RIO_UNUSED(err);
	}

	void Semaphore::wait()
	{
		DWORD result = WaitForSingleObject(semaphoreHandle, INFINITE);
		RIO_ASSERT(result == WAIT_OBJECT_0, "WaitForSingleObject: GetLastError = %d", GetLastError());
		RIO_UNUSED(result);
	}

} // namespace Rio

#endif // RIO_PLATFORM_WINDOWS