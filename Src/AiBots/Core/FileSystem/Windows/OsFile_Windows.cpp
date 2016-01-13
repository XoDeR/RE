// Copyright (c) 2015 Volodymyr Syvochka
#include "Core/FileSystem/OsFile.h"

#if RIO_PLATFORM_WINDOWS

namespace Rio
{
	void OsFile::open(const char* path, FileOpenMode mode)
	{
		file = CreateFile(path,
			(mode == FileOpenMode::Read) ? GENERIC_READ : GENERIC_WRITE,
			0,
			NULL,
			OPEN_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		RIO_ASSERT(file != INVALID_HANDLE_VALUE, "CreateFile: GetLastError = %d", GetLastError());
	}

	void OsFile::close()
	{
		if (isFileOpen())
		{
			CloseHandle(file);
			file = INVALID_HANDLE_VALUE;;
		}
	}

	bool OsFile::isFileOpen() const
	{
		return file != INVALID_HANDLE_VALUE;
	}

	size_t OsFile::read(void* data, size_t size)
	{
		RIO_ASSERT(data != NULL, "Data must be != NULL");
		DWORD bytesRead;
		BOOL result = ReadFile(file, data, size, &bytesRead, NULL);
		RIO_ASSERT(result == TRUE, "ReadFile: GetLastError = %d", GetLastError());
		if (result && bytesRead == 0)
		{
			isEndOfFile = true;
		}
		return bytesRead;
	}

	size_t OsFile::write(const void* data, size_t size)
	{
		RIO_ASSERT(data != NULL, "Data must be != NULL");
		DWORD bytesWritten;
		WriteFile(file, data, size, &bytesWritten, NULL);
		RIO_ASSERT(size == bytesWritten, "WriteFile: GetLastError = %d", GetLastError());
		return bytesWritten;
	}

	void OsFile::flush()
	{
		BOOL err = FlushFileBuffers(file);
		RIO_ASSERT(err != 0, "FlushFileBuffers: GetLastError = %d", GetLastError());
		RIO_UNUSED(err);
	}

	void OsFile::seek(size_t position)
	{
		DWORD err = SetFilePointer(file, position, NULL, FILE_BEGIN);
		RIO_ASSERT(err != INVALID_SET_FILE_POINTER, "SetFilePointer: GetLastError = %d", GetLastError());
		RIO_UNUSED(err);
	}

	void OsFile::seekToEnd()
	{
		DWORD err = SetFilePointer(file, 0, NULL, FILE_END);
		RIO_ASSERT(err != INVALID_SET_FILE_POINTER, "SetFilePointer: GetLastError = %d", GetLastError());
		RIO_UNUSED(err);
	}

	void OsFile::skip(size_t bytes)
	{
		DWORD err = SetFilePointer(file, bytes, NULL, FILE_CURRENT);
		RIO_ASSERT(err != INVALID_SET_FILE_POINTER, "SetFilePointer: GetLastError = %d", GetLastError());
		RIO_UNUSED(err);
	}

	size_t OsFile::getFilePosition() const
	{
		DWORD position = SetFilePointer(file, 0, NULL, FILE_CURRENT);
		RIO_ASSERT(position != INVALID_SET_FILE_POINTER, "SetFilePointer: GetLastError = %d", GetLastError());
		return (size_t)position;
		RIO_UNUSED(position);
	}

	bool OsFile::getIsEndOfFile() const
	{
		return isEndOfFile;
	}

	size_t Rio::OsFile::getFileSize() const
	{
		return GetFileSize(file, NULL);
	}

} // namespace Rio

#endif // RIO_PLATFORM_WINDOWS