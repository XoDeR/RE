// Copyright (c) 2015 Volodymyr Syvochka
#pragma once

#include "Core/Base/Config.h"
#include "Core/Base/Types.h"

namespace Rio
{
	// Network address helper
	struct NetAddress
	{
		// Default address is 127.0.0.1
		NetAddress()
		{
			set(127, 0, 0, 1);
		}

		NetAddress(uint8_t a, uint8_t b, uint8_t c, uint8_t d)
		{
			set(a, b, c, d);
		}

		// Returns the IP address as packed 32-bit integer.
		uint32_t getAddress() const
		{
			return ipAddress;
		}

		void set(uint8_t a, uint8_t b, uint8_t c, uint8_t d)
		{
			ipAddress = uint32_t(a) << 24
				| uint32_t(b) << 16
				| uint32_t(c) << 8
				| uint32_t(d);
		}

		uint32_t ipAddress = 0;
	};

} // namespace Rio
