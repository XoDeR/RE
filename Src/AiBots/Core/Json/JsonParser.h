// Copyright (c) 2015 Volodymyr Syvochka
#pragma once

#include "Core/Base/Config.h"

#include "Core/Base/Types.h"
#include "Core/Json/JsonElement.h"

namespace Rio
{

	class File;

	// Parses JSON documents.
	class JsonParser
	{
	public:
		// Reads the JSON document contained in the C non-null string.
		// The string has to remain valid for the whole parser's existence scope.
		JsonParser(const char* s);
		JsonParser(File& f);
		~JsonParser();
		JsonElement getJsonRoot();
	private:
		bool isFromFile;
		const char* jsonDocument;
	private:
		// Disable copying
		JsonParser(const JsonParser&);
		JsonParser& operator=(const JsonParser&);
	};

} // namespace Rio
