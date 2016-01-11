// Copyright (c) 2015 Volodymyr Syvochka
#pragma once

#include "Core/Base/Config.h"

#include "Core/Strings/DynamicString.h"
#include "Core/Json/JsonTypes.h"

namespace Rio
{

	namespace Json
	{
		// Returns the data type of the JSON string
		JsonValueType::Enum getJsonType(const char* json);
		void parseString(const char* json, DynamicString& string);
		double parseDouble(const char* json);
		int32_t parseInt(const char* json);
		float parseFloat(const char* json);
		bool parseBool(const char* json);
		// Parses the JSON array json and puts it into array as pointers to
		// the corresponding items into the original json string.
		void parseArray(const char* json, JsonArray& array);
		// Parses the JSON object and puts it into object as map from
		// key to pointer to the corresponding value into the original string json.
		void parseObject(const char* json, JsonObject& object);
		// Parses the RJSON-encoded json.
		void parse(const char* json, JsonObject& object);
		// Parses the RJSON-encoded json.
		void parse(Buffer& json, JsonObject& object);
	} // namespace Json

} // namespace Rio
