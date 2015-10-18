// Copyright (c) 2015 Volodymyr Syvochka
#pragma once

#include "Core/Base/Config.h"

#include "Core/Base/Types.h"
#include "Core/Debug/Error.h"
#include "Core/FileSystem/File.h" // FileOpenMode

#if RIO_PLATFORM_POSIX
#include <cstdio>
#elif RIO_PLATFORM_WINDOWS
#include "tchar.h"
#include "Core/Os/Windows/Headers_Windows.h"
#endif

namespace Rio
{
	// OS file wrapper
	class OsFile
	{
	public:
		// Opens the file located at path with the given mode.
		OsFile(const char* path, FileOpenMode mode);
		~OsFile()
		{
			close();
		}
		void close();
		bool isFileOpen() const
		{
			return file != NULL;
		}
		size_t getFileSize() const;
		// Reads size bytes from the file and stores it into data.
		// Returns the number of bytes read.
		size_t read(void* data, size_t size);
		// Writes size bytes of data stored in data 
		// Returns the number of bytes written
		size_t write(const void* data, size_t size);
		// Moves the file pointer to the given position.
		void seek(size_t position);
		// Moves the file pointer to the end of the file.
		void seekToEnd();
		// Moves the file pointer n bytes after the current file pointer position.
		void skip(size_t bytes);
		// Returns the position of the file pointer from the start of the file in bytes
		size_t getFilePosition() const;
		// Returns whether the file pointer is at the end of the file.
		bool getIsEndOfFile() const;
	private:
#if RIO_PLATFORM_POSIX
		FILE* file;
#elif RIO_PLATFORM_WINDOWS
		HANDLE file;
		bool isEndOfFile;
#endif
	};

} // namespace Rio
