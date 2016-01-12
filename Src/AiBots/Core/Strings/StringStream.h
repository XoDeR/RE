// Copyright (c) 2015 Volodymyr Syvochka
#pragma once

#include "Core/Base/Config.h"

#include "Core/Containers/Array.h"
#include "Core/Strings/StringUtils.h"

#include "Core/Math/Vector2.h"
#include "Core/Math/Vector3.h"
#include "Core/Math/Vector4.h"
#include "Core/Math/Quaternion.h"
#include "Core/Math/Matrix4x4.h"

namespace Rio
{
	// Stream of characters.
	using StringStream = Array<char>;

	// Functions to operate on StringStream.
	namespace StringStreamFn
	{
		// Appends a value to the stream s using appropriate formatting.
		StringStream& operator<<(StringStream& s, char ch);
		StringStream& operator<<(StringStream& s, int16_t val);
		StringStream& operator<<(StringStream& s, uint16_t val);
		StringStream& operator<<(StringStream& s, int32_t val);
		StringStream& operator<<(StringStream& s, uint32_t val);
		StringStream& operator<<(StringStream& s, int64_t val);
		StringStream& operator<<(StringStream& s, uint64_t val);
		StringStream& operator<<(StringStream& s, float val);
		StringStream& operator<<(StringStream& s, double val);

		// Appends a C string to the stream
		StringStream& operator<<(StringStream& s, const char* string);

		// Returns the stream as a NULL-terminated C string.
		const char* toCStr(StringStream& s);

		StringStream& operator<<(StringStream& stringStream, const Vector2& v);

		template <typename T> StringStream& streamPrintf(StringStream& s, const char* format, T& val);

		inline StringStream& operator<<(StringStream& s, char ch)
		{
			ArrayFn::pushBack(s, ch);
			return s;
		}

		inline StringStream& operator<<(StringStream& s, int16_t val)
		{
			return streamPrintf(s, "%hd", val);
		}

		inline StringStream& operator<<(StringStream& s, uint16_t val)
		{
			return streamPrintf(s, "%hu", val);
		}

		inline StringStream& operator<<(StringStream& s, int32_t val)
		{
			return streamPrintf(s, "%d", val);
		}

		inline StringStream& operator<<(StringStream& s, uint32_t val)
		{
			return streamPrintf(s, "%u", val);
		}

		inline StringStream& operator<<(StringStream& s, int64_t val)
		{
			return streamPrintf(s, "%lld", val);
		}

		inline StringStream& operator<<(StringStream& s, uint64_t val)
		{
			return streamPrintf(s, "%llu", val);
		}

		inline StringStream& operator<<(StringStream& s, float val)
		{
			return streamPrintf(s, "%g", val);
		}

		inline StringStream& operator<<(StringStream& s, double val)
		{
			return streamPrintf(s, "%g", val);
		}

		inline StringStream& operator<<(StringStream& s, const char* str)
		{
			ArrayFn::push(s, str, strLen(str));
			return s;
		}

		inline const char* toCStr(StringStream& s)
		{
			ArrayFn::pushBack(s, '\0');
			ArrayFn::popBack(s);
			return ArrayFn::begin(s);
		}

		template <typename T>
		inline StringStream& streamPrintf(StringStream& stringStream, const char* format, T& val)
		{
			char buf[32];
			snPrintf(buf, sizeof(buf), format, val);
			return stringStream << buf;
		}

		inline StringStream& operator<<(StringStream& stringStream, const Vector2& v)
		{
			stringStream << "Vector2{" << v.x << ", " << v.y << "}";
			return stringStream;
		}

		inline StringStream& operator<<(StringStream& stringStream, const Vector3& v)
		{
			return stringStream << "Vector3{" << v.x << ", " << v.y << ", " << v.z << "}";
		}

		inline StringStream& operator<<(StringStream& stringStream, const Vector4& v)
		{
			return stringStream << "Vector4{" << v.x << ", " << v.y << ", " << v.z << ", " << v.w
				<< "}";
		}

		inline StringStream& operator<<(StringStream& stringStream, const Quaternion& q)
		{
			stringStream << "Quaternion{";
			for (size_t i = 0; i < 4; i++)
			{
				stringStream << q.data[i];
				if (i < 3)
				{
					stringStream << ", ";
				}
			}
			stringStream << "}";

			return stringStream;
		}

		inline StringStream& operator<<(StringStream& stringStream, const Matrix4x4& m)
		{
			stringStream << "Matrix4x4{";
			for (size_t i = 0; i < 4; i++)
			{
				stringStream << "\n\t";
				for (size_t j = 0; j < 4; j++)
				{
					stringStream << m[i * 4 + j];
					if (j < 3)
					{
						stringStream << ", ";
					}
				}
				if (i < 3)
				{
					stringStream << ", ";
				}
			}
			stringStream << "\n}";

			return stringStream;
		}
	} // namespace StringStreamFn

} // namespace Rio
