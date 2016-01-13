// Copyright (c) 2015 Volodymyr Syvochka
#pragma once

#include "Core/Base/Config.h"
#include "Core/Base/Types.h"

namespace Rio
{
	enum class FileOpenMode
	{
		Read = 1,
		Write = 2
	};

	// An abstraction to access data
	// It represents a flow of data attached to a file which can be:
	// 1. archived file
	// 2. regular file
	// 3. location in memory
	// 4. anything that can be read or written
	class File
	{
	public:
		File() 
		{}
		virtual ~File() 
		{};

		virtual void open(const char* path, FileOpenMode mode) = 0;
		virtual void close() = 0;
		// Sets the position indicator of the file to position.
		virtual void seek(size_t position) = 0;
		// Sets the position indicator to the end of the file
		virtual void seekToEnd() = 0;
		// Sets the position indicator to bytes after current position
		virtual void skip(size_t bytes) = 0;
		// Reads a block of data from the file.
		virtual size_t read(void* data, size_t size) = 0;
		// Writes a block of data to the file.
		virtual size_t write(const void* data, size_t size) = 0;
		// Forces the previous write operations to complete.
		// Write operations in the file system are not performed instantly
		// the output data may be stored to a temporary buffer before making its way to the file. 
		// This method forces all the pending output operations to be written to the file.
		virtual void flush() = 0;
		// Returns whether the file is valid.
		// A file is valid when the buffer where it operates exists 
		// (a file descriptor is attached to the file, a memory area is attached to the file etc.)
		virtual bool isValid() = 0;
		// Returns whether the position is at end of file.
		virtual bool getIsEndOfFile() = 0;
		virtual size_t getFileSize() = 0;
		// Returns the current position in file.
		// For binary data, it means the number of bytes
		// from the beginning of the file.
		virtual size_t getFilePosition() = 0;
	};

} // namespace Rio
