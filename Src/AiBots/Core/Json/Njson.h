// Copyright (c) 2015 Volodymyr Syvochka
#pragma once

#include "Core/Base/Config.h"

#include "Core/Strings/DynamicString.h"
#include "Core/Containers/Map.h"

namespace Rio
{

	// Enumerates JSON value types.
	struct NjsonValueType
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

	// Functions to parse NJSON-encoded strings.
	namespace Njson
	{
		// Returns the data type of the NJSON string
		NjsonValueType::Enum getJsonType(const char* json);
		void parseString(const char* json, DynamicString& string);
		double parseDouble(const char* json);
		int32_t parseInt(const char* json);
		float parseFloat(const char* json);
		bool parseBool(const char* json);
		// Parses the NJSON array and puts it into array as pointers to
		// the corresponding items into the original json string.
		void parseArray(const char* json, Array<const char*>& array);
		// Parses the NJSON object and puts it into object as map from
		// key to pointer to the corresponding value into the original string json.
		void parseObject(const char* json, Map<DynamicString, const char*>& object);
	} // namespace Njson

} // namespace Rio
