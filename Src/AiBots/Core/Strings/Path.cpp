// Copyright (c) 2015 Volodymyr Syvochka
#include "Path.h"

#include "Core/Strings/StringUtils.h"

#include <ctype.h> // isalpha
#include <string.h> // strlen, strrchr

namespace Rio
{

	namespace PathFn
	{
		bool isAbsolutePath(const char* path)
		{
			RIO_ASSERT(path != NULL, "Path must be != NULL");
#if RIO_PLATFORM_POSIX
			return strLen32(path) > 0 && path[0] == SEPARATOR;
#elif RIO_PLATFORM_WINDOWS
			return strLen32(path) > 2 && isalpha(path[0]) && path[1] == ':' && path[2] == SEPARATOR;
#endif
		}

		bool isRootPath(const char* path)
		{
			RIO_ASSERT(path != NULL, "Path must be != NULL");
#if RIO_PLATFORM_POSIX
			return isAbsolutePath(path) && strLen32(path) == 1;
#elif RIO_PLATFORM_WINDOWS
			return isAbsolutePath(path) && strLen32(path) == 3;
#endif
		}

		void joinPaths(const char* a, const char* b, DynamicString& path)
		{
			const uint32_t la = strLen32(a);
			const uint32_t lb = strLen32(b);
			path.reserve(la + lb + 1);
			path += a;
			path += SEPARATOR;
			path += b;
		}

		const char* normalizePath(const char* path)
		{
#if RIO_PLATFORM_POSIX
			static char norm[1024];
			char* cur = norm;

			while ((*path) != '\0')
			{
				if ((*path) == '\\')
				{
					(*cur) = SEPARATOR;
				}
				else
				{
					(*cur) = (*path);
				}

				path++;
				cur++;
			}

			return norm;
#elif RIO_PLATFORM_WINDOWS
			static char norm[1024];

			for (uint32_t i = 0; i < strLen32(path) + 1; i++)
			{
				if (path[i] == '/')
				{
					norm[i] = SEPARATOR;
				}
				else
				{
					norm[i] = path[i];
				}
			}

			return norm;
#endif
		}

		void getPathNameFromPath(const char* path, char* str, size_t len)
		{
			RIO_ASSERT(path != NULL, "Path must be != NULL");
			RIO_ASSERT(str != NULL, "Str must be != NULL");

			const char* lastSeparator = findLast(path, '/');

			if (lastSeparator == end(path))
			{
				strnCpy(str, "", len);
			}
			else
			{
				substring(begin(path), lastSeparator, str, len);
			}
		}


		void getFileNameFromPath(const char* path, char* str, size_t len)
		{
			RIO_ASSERT(path != NULL, "Path must be != NULL");
			RIO_ASSERT(str != NULL, "Str must be != NULL");

			const char* lastSeparator = findLast(path, '/');

			if (lastSeparator == end(path))
			{
				strnCpy(str, "", len);
			}
			else
			{
				substring(lastSeparator + 1, end(path), str, len);
			}
		}

		void getBaseNameFromPath(const char* path, char* str, size_t len)
		{
			RIO_ASSERT(path != NULL, "Path must be != NULL");
			RIO_ASSERT(str != NULL, "Str must be != NULL");

			const char* lastSeparator = findLast(path, '/');
			const char* lastDot = findLast(path, '.');

			if (lastSeparator == end(path) && lastDot != end(path))
			{
				substring(begin(path), lastDot, str, len);
			}
			else if (lastSeparator != end(path) && lastDot == end(path))
			{
				substring(lastSeparator + 1, end(path), str, len);
			}
			else if (lastSeparator == end(path) && lastDot == end(path))
			{
				strnCpy(str, path, len);
			}
			else
			{
				substring(lastSeparator + 1, lastDot, str, len);
			}
		}

		const char* getBaseNameFromPath(const char* path)
		{
			const char* lastSlashPosition = ::strrchr(path, '/');
			return lastSlashPosition == NULL ? path : lastSlashPosition + 1;
		}

		void getExtensionFromPath(const char* path, char* str, size_t len)
		{
			RIO_ASSERT(path != NULL, "Path must be != NULL");
			RIO_ASSERT(str != NULL, "Str must be != NULL");

			const char* lastDot = findLast(path, '.');

			if (lastDot == end(path))
			{
				strnCpy(str, "", len);
			}
			else
			{
				substring(lastDot + 1, end(path), str, len);
			}
		}

		const char* getExtensionFromPath(const char* path)
		{
			const char* lastDotPosition = ::strrchr(path, '.');
			return lastDotPosition == NULL ? NULL : lastDotPosition + 1;
		}

		void getFileNameWithoutExtensionFromPath(const char* path, char* str, size_t len)
		{
			RIO_ASSERT(path != NULL, "Path must be != NULL");
			RIO_ASSERT(str != NULL, "Str must be != NULL");

			const char* lastDot = findLast(path, '.');

			substring(begin(path), lastDot, str, len);
		}

		void stripTrailingSeparator(const char* path, char* str, size_t len)
		{
			RIO_ASSERT(path != NULL, "Path must be != NULL");
			RIO_ASSERT(str != NULL, "Str must be != NULL");

			size_t path_len = strLen(path);

			if (path[path_len - 1] == '/')
			{
				substring(begin(path), end(path) - 2, str, len);
			}
			else
			{
				substring(begin(path), end(path), str, len);
			}
		}
	} // namespace PathFn

} // namespace Rio
