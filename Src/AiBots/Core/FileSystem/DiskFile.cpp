// Copyright (c) 2015 Volodymyr Syvochka
#include "DiskFile.h"

#include "Core/Base/Types.h"
#include "Core/Memory/Memory.h"
#include <algorithm>

namespace Rio
{
	DiskFile::DiskFile()
	{
	}

	DiskFile::~DiskFile()
	{
		file.close();
	}

	void DiskFile::open(const char* path, FileOpenMode mode)
	{
		file.open(path, mode);
	}

	void DiskFile::close()
	{
		file.close();
	}

	void DiskFile::seek(size_t position)
	{
		checkIsFileValid();
		file.seek(position);
	}

	void DiskFile::seekToEnd()
	{
		checkIsFileValid();
		file.seekToEnd();
	}

	void DiskFile::skip(size_t bytes)
	{
		checkIsFileValid();
		file.skip(bytes);
	}

	size_t DiskFile::read(void* data, size_t size)
	{
		checkIsFileValid();
		return file.read(data, size);
	}

	size_t DiskFile::write(const void* data, size_t size)
	{
		checkIsFileValid();
		return file.write(data, size);
	}

	bool DiskFile::getIsEndOfFile()
	{
		return getFilePosition() == getFileSize();
	}

	bool DiskFile::isValid()
	{
		return file.isFileOpen();
	}

	void DiskFile::flush()
	{
		checkIsFileValid();
		file.flush();
	}

	size_t DiskFile::getFilePosition()
	{
		checkIsFileValid();
		return file.getFilePosition();
	}

	size_t DiskFile::getFileSize()
	{
		checkIsFileValid();
		return file.getFileSize();
	}

} // namespace Rio
