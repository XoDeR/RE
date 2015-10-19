// Copyright (c) 2015 Volodymyr Syvochka
#pragma once

#include "Core/Base/Config.h"

#include "Core/Base/Types.h"

namespace Rio
{
	struct Id
	{
		static const uint16_t idInvalid = 65535;
		uint16_t id;
		uint16_t index;

		void decode(uint32_t idAndIndex)
		{
			id = (idAndIndex & 0xFFFF0000) >> 16;
			index = idAndIndex & 0xFFFF;
		}

		uint32_t encode() const
		{
			return (uint32_t(id) << 16) | uint32_t(index);
		}

		bool operator==(const Id& other)
		{
			return id == other.id && index == other.index;
		}

		bool operator!=(const Id& other)
		{
			return id != other.id || index != other.index;
		}
	};

} // namespace Rio