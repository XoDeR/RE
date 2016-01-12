// Copyright (c) 2015 Volodymyr Syvochka
#pragma once

#include "Core/Base/Config.h"
#include "Core/Base/Types.h"

namespace Rio
{

	struct StringId32
	{
		uint32_t id;

		StringId32() 
			: id(0) 
		{}
		explicit StringId32(uint32_t id)
			: id(id)
		{}
		explicit StringId32(const char* str);
		StringId32(const char* str, uint32_t len);
		StringId32 operator=(StringId32 a)
		{
			id = a.id; return *this;
		}
		bool operator==(StringId32 a) const
		{
			return id == a.id;
		}
		bool operator!=(StringId32 a) const
		{
			return id != a.id;
		}
		bool operator<(StringId32 a) const
		{
			return id < a.id;
		}
		uint32_t getId() const
		{
			return id;
		}
		const char* toString(char* buf);

		static const uint32_t STRING_LENGTH = 32;
	};

} //namespace Rio