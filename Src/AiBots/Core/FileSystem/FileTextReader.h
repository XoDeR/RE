// Copyright (c) 2015 Volodymyr Syvochka
#pragma once

#include "Core/Base/Config.h"

#include "Core/Base/Types.h"
#include "Core/FileSystem/File.h"

namespace Rio
{
	// A reader that offers a convenient way to read text from a File
	class FileTextReader
	{
	public:
		FileTextReader(File& file) 
			: file(file) {}

		// Reads characters from file and stores them as a C string
		// until (size-1) characters have been read or a newline or EOF is reached, whichever comes first.
		// A newline character makes fgets stop reading, but it is considered
		// a valid character and therefore it is included in the string copied to string.
		// A null character is automatically appended in str after the characters read to
		// signal the end of the C string.
		size_t readString(char* string, size_t size)
		{
			char currentChar;
			size_t bytesRead = 0;

			while (!file.getIsEndOfFile() && bytesRead < size - 1)
			{
				file.read(&currentChar, 1);
				string[bytesRead] = currentChar;

				bytesRead++;

				if (currentChar == '\n')
				{
					break;
				}
			}

			string[bytesRead] = '\0';
			return bytesRead;
		}
	private:
		File& file;
	};
} // namespace Rio