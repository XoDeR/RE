// Copyright (c) 2015 Volodymyr Syvochka
#pragma once

#include "Core/Base/Config.h"

namespace Rio
{

	namespace ErrorFn
	{
		// Aborts the program execution logging an error message and the stacktrace if the platform supports it
		void abort(const char* file, int line, const char* message, ...);
	} // namespace ErrorFn

} // namespace Rio

#if RIO_DEBUG
#define RIO_ASSERT(condition, msg, ...) do { if (!(condition)) {\
		Rio::ErrorFn::abort(__FILE__, __LINE__, "\nAssertion failed: %s\n\t" msg "\n", #condition, ##__VA_ARGS__); }} while (0)
#else
#define RIO_ASSERT(...) ((void)0)
#endif // RIO_DEBUG

#define RIO_ASSERT_NOT_NULL(x) RIO_ASSERT(x != NULL, #x " must be not null")
#define RIO_FATAL(msg) RIO_ASSERT(false, msg)
