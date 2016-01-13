// Copyright (c) 2015 Volodymyr Syvochka
#pragma once

#include "Core/Base/Config.h"

#include "Core/Base/Types.h"
#include "Core/Strings/StringUtils.h"

#include "Core/FileSystem/File.h"

namespace Rio
{
	// A reader that offers a convenient way to write text to a File
	class FileTextWriter
	{
	public:
		FileTextWriter(File& file) : file(file) {}
		// Writes the string pointed by string to the file.
		// The function begins copying from the address specified (string)
		// until it reaches the terminating null character ('\0').
		// The final null character is not copied to the file.
		void writeString(const char* string)
		{
			file.write(string, Rio::strLen(string));
		}
	private:
		File& file;
	};
} // namespace Rio