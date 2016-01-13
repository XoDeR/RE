// Copyright (c) 2015 Volodymyr Syvochka
#include "DiskFileSystem.h"

#include "Core/Strings/StringUtils.h"
#include "Core/Strings/Path.h"
#include "Core/Memory/TempAllocator.h"
#include "Core/FileSystem/DiskFile.h"
#include "Core/FileSystem/File.h"
#include "Core/Containers/Vector.h"
#include "Core/Os/Os.h"

namespace Rio
{

	DiskFilesystem::DiskFilesystem()
	{
		char buf[512];
		Os::getCurrentDirName(buf, sizeof(buf));
		fileSystemPrefix = buf;
	}

	DiskFilesystem::DiskFilesystem(const char* prefix)
		: fileSystemPrefix(prefix)
	{
	}

	File* DiskFilesystem::fileOpen(const char* path, FileOpenMode mode)
	{
		RIO_ASSERT_NOT_NULL(path);

		TempAllocator256 alloc;
		DynamicString absPath(alloc);
		getAbsolutePath(path, absPath);

		DiskFile* file = getDefaultAllocator().makeNew<DiskFile>();
		file->open(absPath.toCStr(), mode);
		return file;
	}

	void DiskFilesystem::fileClose(File& file)
	{
		getDefaultAllocator().makeDelete(&file);
	}

	bool DiskFilesystem::doesPathExist(const char* path)
	{
		RIO_ASSERT_NOT_NULL(path);

		TempAllocator256 alloc;
		DynamicString absPath(alloc);
		getAbsolutePath(path, absPath);

		return Os::doesPathExist(absPath.toCStr());
	}

	bool DiskFilesystem::isDirectory(const char* path)
	{
		RIO_ASSERT_NOT_NULL(path);

		TempAllocator256 alloc;
		DynamicString absPath(alloc);
		getAbsolutePath(path, absPath);

		return Os::isDirectory(absPath.toCStr());
	}

	bool DiskFilesystem::isFile(const char* path)
	{
		RIO_ASSERT_NOT_NULL(path);

		TempAllocator256 alloc;
		DynamicString absPath(alloc);
		getAbsolutePath(path, absPath);

		return Os::isFile(absPath.toCStr());
	}

	uint64_t DiskFilesystem::getLastModifiedTime(const char* path)
	{
		RIO_ASSERT_NOT_NULL(path);

		TempAllocator256 alloc;
		DynamicString absolutePath(alloc);
		getAbsolutePath(path, absolutePath);

		return Os::getLastModifiedTime(absolutePath.toCStr());
	}

	void DiskFilesystem::createDirectory(const char* path)
	{
		RIO_ASSERT_NOT_NULL(path);

		TempAllocator256 alloc;
		DynamicString absPath(alloc);
		getAbsolutePath(path, absPath);

		if (!Os::doesPathExist(absPath.toCStr()))
		{
			Os::createDirectory(absPath.toCStr());
		}
	}

	void DiskFilesystem::deleteDirectory(const char* path)
	{
		RIO_ASSERT_NOT_NULL(path);

		TempAllocator256 alloc;
		DynamicString absPath(alloc);
		getAbsolutePath(path, absPath);

		Os::deleteDirectory(absPath.toCStr());
	}

	void DiskFilesystem::createFile(const char* path)
	{
		RIO_ASSERT_NOT_NULL(path);

		TempAllocator256 alloc;
		DynamicString absPath(alloc);
		getAbsolutePath(path, absPath);

		Os::createFile(absPath.toCStr());
	}

	void DiskFilesystem::deleteFile(const char* path)
	{
		RIO_ASSERT_NOT_NULL(path);

		TempAllocator256 alloc;
		DynamicString absPath(alloc);
		getAbsolutePath(path, absPath);

		Os::deleteFile(absPath.toCStr());
	}

	void DiskFilesystem::getFileList(const char* path, Vector<DynamicString>& files)
	{
		RIO_ASSERT_NOT_NULL(path);

		TempAllocator256 alloc;
		DynamicString absPath(alloc);
		getAbsolutePath(path, absPath);

		Os::getFileList(absPath.toCStr(), files);
	}

	void DiskFilesystem::getAbsolutePath(const char* path, DynamicString& osPath)
	{
		if (PathFn::isAbsolutePath(path))
		{
			osPath = path;
			return;
		}

		PathFn::joinPaths(fileSystemPrefix.toCStr(), path, osPath);
	}

	

} // namespace Rio
