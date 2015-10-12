// Copyright (c) 2015 Volodymyr Syvochka
#include "Core/Debug/Error.h"
#include "StackTrace.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

namespace Rio
{

namespace ErrorFn
{
	void abort(const char* file, int line, const char* message, ...)
	{
		va_list ap;
		va_start(ap, message);
		vprintf(message, ap);
		va_end(ap);
		printf("\tIn: %s:%d\n", file, line);
		printf("Stacktrace:\n");
		stacktrace();
		exit(EXIT_FAILURE);
	}
} // namespace ErrorFn

} // namespace Rio
