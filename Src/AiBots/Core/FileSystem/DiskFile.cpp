// Copyright (c) 2015 Volodymyr Syvochka
#include "DiskFile.h"

#include "Core/Base/Types.h"
#include "Core/Debug/Log.h"
#include "Core/Math/MathUtils.h"
#include "Core/Memory/Memory.h"

namespace Rio 
{

DiskFile::DiskFile(FileOpenMode mode, const char* filename)
	: File(mode)
	, file(filename, mode)
	, lastOperationWasRead(true)
{
}

DiskFile::~DiskFile()
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

size_t DiskFile::read(void* buffer, size_t size)
{
	checkIsFileValid();
	if (!lastOperationWasRead)
	{
		lastOperationWasRead = true;
		file.seek(0);
	}
	size_t bytesRead = file.read(buffer, size);
	RIO_ASSERT(bytesRead == size, "Failed to read from file: requested: %llu, read: %llu", size, bytesRead);
	return bytesRead;
}

void DiskFile::write(const void* buffer, size_t size)
{
	checkIsFileValid();
	if (lastOperationWasRead)
	{
		lastOperationWasRead = false;
		file.seek(0);
	}
	size_t bytesWritten = file.write(buffer, size);
	RIO_ASSERT(bytesWritten == size, "Failed to write to file: requested: %llu, written: %llu", size, bytesWritten);
}

bool DiskFile::copyToFile(File& file, size_t size)
{
	checkIsFileValid();
	const size_t chunksize = 1024*1024;

	char* buff = (char*) getDefaultAllocator().allocate(chunksize * sizeof(char));

	size_t totalBytesRead = 0;

	while (totalBytesRead < size)
	{
		size_t bytesRead;
		size_t expectedBytesRead = min(size - totalBytesRead, chunksize);

		bytesRead = file.read(buff, expectedBytesRead);

		if (bytesRead < expectedBytesRead)
		{
			if (file.getIsEndOfFile())
			{
				if (bytesRead != 0)
				{
					file.write(buff, bytesRead);
				}
			}

			getDefaultAllocator().deallocate(buff);
			//Either the file gave an error, or ended before size bytes could be copied
			return false;
		}

		file.write(buff, bytesRead);
		totalBytesRead += bytesRead;
	}

	getDefaultAllocator().deallocate(buff);
	return true;
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
	// not implemented
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

bool DiskFile::canReadFile() const
{
	checkIsFileValid();
	return true;
}

bool DiskFile::canWriteFile() const
{
	checkIsFileValid();
	return true;
}

bool DiskFile::canSeekFile() const
{
	return true;
}

} // namespace Rio
