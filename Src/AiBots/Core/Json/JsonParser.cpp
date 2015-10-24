// Copyright (c) 2015 Volodymyr Syvochka
#include "JsonParser.h"

#include "Core/Json/Njson.h"
#include "Core/Memory/TempAllocator.h"
#include "Core/Strings/StringUtils.h"
#include "Core/FileSystem/File.h"

namespace Rio
{

	JsonParser::JsonParser(const char* s)
		: isFromFile(false)
		, jsonDocument(s)
	{
		RIO_ASSERT_NOT_NULL(s);
	}

	JsonParser::JsonParser(File& f)
		: isFromFile(true)
		, jsonDocument(NULL)
	{
		const size_t size = f.getFileSize();
		char* doc = (char*)getDefaultAllocator().allocate(size);
		f.read(doc, size);
		jsonDocument = doc;
	}

	JsonParser::~JsonParser()
	{
		if (isFromFile)
		{
			getDefaultAllocator().deallocate((void*)jsonDocument);
		}
	}

	JsonElement JsonParser::getJsonRoot()
	{
		const char* ch = jsonDocument;
		while ((*ch) && (*ch) <= ' ') ch++;

		return JsonElement(ch);
	}

} // namespace Rio
