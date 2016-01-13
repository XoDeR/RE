// Copyright (c) 2015 Volodymyr Syvochka
#pragma once

#include "Core/Base/Config.h"

#include "Core/Debug/Error.h"
#include "Core/FileSystem/OsFile.h"
#include "Core/FileSystem/File.h"

namespace Rio
{
	// file on disk.
	class DiskFile : public File
	{
	public:
		DiskFile();
		virtual ~DiskFile();
		void open(const char* path, FileOpenMode mode);
		void close();
		void seek(size_t position);
		void seekToEnd();
		void skip(size_t bytes);
		size_t read(void* data, size_t size);
		size_t write(const void* data, size_t size);
		void flush();
		bool getIsEndOfFile();
		bool isValid();
		size_t getFileSize();
		size_t getFilePosition();
	protected:
		OsFile file;
	protected:
		inline void checkIsFileValid() const
		{
			RIO_ASSERT(file.isFileOpen(), "File is not open");
		}
	};

} // namespace Rio
