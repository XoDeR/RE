// Copyright (c) 2015 Volodymyr Syvochka
#pragma once

#include "Core/Base/Config.h"

#include "Core/Debug/Error.h"
#include "Core/Base/Types.h"

#include <cstdio>
#include <cstring>
#include <cstdarg>
#include <cctype>

// wrappers for C string functions
// arguments are char* or const char* C zero-ended strings

namespace Rio 
{

	inline size_t strLen(const char* str)
{
	return ::strlen(str);
}

inline const char* strStr(const char* str1, const char* str2)
{
	return ::strstr(str1, str2);
}

inline int32_t strCmp(const char* str1, const char* str2)
{
	return ::strcmp(str1, str2);
}

inline int32_t strnCmp(const char* s1, const char* s2, size_t len)
{
	return ::strncmp(s1, s2, len);
}

inline char* strnCpy(char* dest, const char* src, size_t len)
{
	char* ret = ::strncpy(dest, src, len);
	dest[len - 1] = '\0';

	return ret;
}

inline char* strCat(char* dest, const char* src)
{
	return ::strcat(dest, src);
}

inline char* strnCat(char* dest, const char* src, size_t len)
{
	return ::strncat(dest, src, len);
}

inline int32_t vsnPrintf(char* str, size_t num, const char* format, va_list args)
{
#if RIO_COMPILER_MSVC
	int32_t len = _vsnprintf_s(str, num, _TRUNCATE, format, args);
	return (len == 1) ? _vscprintf(format, args) : len;
#else
	return ::vsnprintf(str, num, format, args);
#endif // RIO_COMPILER_MSVC
}

inline int32_t snPrintf(char* str, size_t n, const char* format, ...)
{
	va_list args;
	va_start(args, format);
	int32_t len = vsnPrintf(str, n, format, args);
	va_end(args);
	return len;
}

inline const char* skipSpaces(const char* str)
{
	while (isspace(*str))
	{
		++str;
	}
	return str;
}

inline const char* skipBlock(const char* str, char a, char b)
{
	uint32_t num = 0;

	for (char ch = *str++; *str != '\0'; ch = *str++)
	{
		if (ch == a) ++num;
		else if (ch == b)
		{
			if (--num == 0)
			{
				return str;
			}
		}
	}

	return nullptr;
}

inline const char* begin(const char* str)
{
	RIO_ASSERT(str != nullptr, "String is NULL");
	return str;
}

inline const char* end(const char* str)
{
	RIO_ASSERT(str != nullptr, "String is NULL");
	return str + strLen(str) + 1;
}

inline const char* findFirst(const char* str, char c)
{
	RIO_ASSERT(str != nullptr, "String is NULL");
	const char* str_begin = begin(str);

	while (str_begin != end(str))
	{
		if ((*str_begin) == c)
		{
			return str_begin;
		}

		str_begin++;
	}

	return end(str);
}

inline const char* findLast(const char* str, char c)
{
	RIO_ASSERT(str != NULL, "Str must be != NULL");

	const char* str_end = end(str) - 1;

	while (str_end != begin(str) - 1)
	{
		if ((*str_end) == c)
		{
			return str_end;
		}

		str_end--;
	}

	return end(str);
}

inline void substring(const char* begin, const char* end, char* out, size_t len)
{
	RIO_ASSERT(begin != NULL, "Begin must be != NULL");
	RIO_ASSERT(end != NULL, "End must be != NULL");
	RIO_ASSERT(out != NULL, "Out must be != NULL");

	size_t i = 0;

	char* out_iterator = out;

	while (begin != end && i < len)
	{
		(*out_iterator) = (*begin);

		begin++;
		out_iterator++;
		i++;
	}

	out[i] = '\0';
}

inline int32_t parseInt(const char* string)
{
	int val;
	int ok = sscanf(string, "%d", &val);

	RIO_ASSERT(ok == 1, "Failed to parse int: %s", string);
	RIO_UNUSED(ok);

	return val;
}

inline uint32_t parseUint(const char* string)
{
	unsigned int val;
	int ok = sscanf(string, "%u", &val);

	RIO_ASSERT(ok == 1, "Failed to parse uint: %s", string);
	RIO_UNUSED(ok);

	return val;
}

inline float parseFloat(const char* string)
{
	float val;
	int ok = sscanf(string, "%f", &val);

	RIO_ASSERT(ok == 1, "Failed to parse float: %s", string);
	RIO_UNUSED(ok);

	return val;
}

inline double parseDouble(const char* string)
{
	double val;
	int ok = sscanf(string, "%lf", &val);

	RIO_ASSERT(ok == 1, "Failed to parse float: %s", string);
	RIO_UNUSED(ok);

	return val;
}

} // namespace Rio
