// Copyright (c) 2015 Volodymyr Syvochka
#pragma once

#include "Core/Base/Config.h"

#include "Core/Debug/Error.h"
#include "Core/FileSystem/OsFile.h"
#include "Core/FileSystem/File.h"

namespace Rio 
{

// Provides common facilities to access files on disk.
class DiskFile: public File
{
public:
	// Opens filename with specified mode
	DiskFile(FileOpenMode mode, const char* filename);
	virtual ~DiskFile();
	void seek(size_t position);
	void seekToEnd();
	void skip(size_t bytes);
	size_t read(void* buffer, size_t size);
	void write(const void* buffer, size_t size);
	bool copyToFile(File& file, size_t size = 0);
	void flush();
	bool getIsEndOfFile();
	bool isValid();
	size_t getFileSize();
	size_t getFilePosition();
	bool canReadFile() const;
	bool canWriteFile() const;
	bool canSeekFile() const;
protected:
	OsFile file;
	bool lastOperationWasRead;
protected:
	inline void checkIsFileValid() const
	{
		RIO_ASSERT(file.isFileOpen(), "File is not open");
	}
};

} // namespace Rio
