// Copyright (c) 2015 Volodymyr Syvochka
#pragma once

#include "Core/Strings/StringUtils.h"
#include <algorithm>

namespace Rio
{

	class FixedString
	{
	public:
		FixedString()
		{
		}

		FixedString(const char* str)
			: length(strLen32(str))
			, data(str)
		{
		}

		FixedString(const char* str, uint32_t len)
			: length(len)
			, data(str)
		{
		}

		FixedString(const FixedString& b)
			: length(b.length)
			, data(b.data)
		{
		}

		FixedString& operator=(const char* str)
		{
			length = strLen32(str);
			data = str;
			return *this;
		}

		bool operator==(const char* str) const
		{
			const uint32_t len = strLen32(str);
			return length == len && !strnCmp(data, str, len);
		}

		bool operator==(const FixedString& b) const
		{
			return (length == b.length) && !strnCmp(data, b.data, length);
		}

		bool operator<(const FixedString& b) const
		{
			const uint32_t len = (std::max)(length, b.length);
			return strnCmp(data, b.data, len) < 0;
		}

	private:
		uint32_t length = 0;
		const char* data = nullptr;
	};

} // namespace Rio
