// Copyright (c) 2015 Volodymyr Syvochka
#pragma once

#include "Core/Base/Config.h"
#include "Core/Thread/Mutex.h"

namespace Rio
{

// RAII wrapper, like std::lock_guard
class ScopedMutex
{
public:
	ScopedMutex(Mutex& m)
		: mutex(m)
	{
		mutex.lock();
	}

	~ScopedMutex()
	{
		mutex.unlock();
	}
private:
	Mutex& mutex;
private:
	// Disable copying
	ScopedMutex(const ScopedMutex&);
	ScopedMutex& operator=(const ScopedMutex&);
};
} // namespace Rio