#include "Core/FileSystem/OsFile.h"

#if RIO_PLATFORM_WINDOWS

namespace Rio
{

	OsFile::OsFile(const char* path, FileOpenMode mode)
		: isEndOfFile(false)
	{
		file = CreateFile(path,
			mode == FOM_READ ? GENERIC_READ : GENERIC_WRITE,
			0,
			NULL,
			OPEN_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		RIO_ASSERT(file != INVALID_HANDLE_VALUE, "Unable to open file: %s", path);
	}

	void OsFile::close()
	{
		if (isFileOpen())
		{
			CloseHandle(file);
			file = NULL;
		}
	}

	size_t OsFile::read(void* data, size_t size)
	{
		RIO_ASSERT(data != NULL, "Data must be != NULL");
		DWORD bytes_read;

		BOOL result = ReadFile(file, data, size, &bytes_read, NULL);

		RIO_ASSERT(result == TRUE, "Unable to read from file");

		if (result && bytes_read == 0)
		{
			isEndOfFile = true;
		}
		return bytes_read;
	}

	size_t OsFile::write(const void* data, size_t size)
	{
		RIO_ASSERT(data != NULL, "Data must be != NULL");
		DWORD bytes_written;
		WriteFile(file, data, size, &bytes_written, NULL);
		RIO_ASSERT(size == bytes_written, "Cannot read from file\n");
		return size;
	}

	void OsFile::seek(size_t position)
	{
		DWORD err = SetFilePointer(file, position, NULL, FILE_BEGIN);
		RIO_ASSERT(err != INVALID_SET_FILE_POINTER, "Failed to seek");
		RIO_UNUSED(err);
	}

	void OsFile::seekToEnd()
	{
		DWORD seek_result = SetFilePointer(file, 0, NULL, FILE_END);
		RIO_ASSERT(seek_result != INVALID_SET_FILE_POINTER, "Failed to seek to end");
	}

	void OsFile::skip(size_t bytes)
	{
		DWORD seek_result = SetFilePointer(file, bytes, NULL, FILE_CURRENT);
		RIO_ASSERT(seek_result != INVALID_SET_FILE_POINTER, "Failed to skip");
	}

	size_t OsFile::getFilePosition() const
	{
		DWORD position = SetFilePointer(file, 0, NULL, FILE_CURRENT);
		RIO_ASSERT(position != INVALID_SET_FILE_POINTER, "Failed to get position");
		return position;
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