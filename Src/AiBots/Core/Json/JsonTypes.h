// Copyright (c) 2015 Volodymyr Syvochka
#pragma once

#include "Core/Base/Config.h"

#include "Core/Containers/Array.h"
#include "Core/Containers/Map.h"
#include "Core/Strings/FixedString.h"

namespace Rio
{
	struct JsonValueType
	{
		enum Enum
		{
			NIL,
			BOOL,
			NUMBER,
			STRING,
			ARRAY,
			OBJECT
		};
	};

	// Array of pointers to json-encoded strings.
	using JsonArray = Array<const char*>;

	// Map from key to pointers to json-encoded strings.
	using JsonObject = Map<FixedString, const char*>;

} // namespace Rio
