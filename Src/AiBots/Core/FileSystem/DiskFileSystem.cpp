// Copyright (c) 2015 Volodymyr Syvochka
#include "DiskFileSystem.h"

#include "Core/Strings/StringUtils.h"
#include "Core/Memory/TempAllocator.h"
#include "Core/FileSystem/DiskFile.h"
#include "Core/Containers/Vector.h"
#include "Core/Strings/Path.h"
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

File* DiskFilesystem::open(const char* path, FileOpenMode mode)
{
	RIO_ASSERT_NOT_NULL(path);

	TempAllocator256 alloc;
	DynamicString absPath(alloc);
	getAbsolutePath(path, absPath);

	return getDefaultAllocator().makeNew<DiskFile>(mode, absPath.toCStr());
}

void DiskFilesystem::close(File* file)
{
	RIO_ASSERT_NOT_NULL(file);
	getDefaultAllocator().makeDelete(file);
}

bool DiskFilesystem::exists(const char* path)
{
	RIO_ASSERT_NOT_NULL(path);

	TempAllocator256 alloc;
	DynamicString absPath(alloc);
	getAbsolutePath(path, absPath);

	return Os::exists(absPath.toCStr());
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

void DiskFilesystem::createDirectory(const char* path)
{
	RIO_ASSERT_NOT_NULL(path);

	TempAllocator256 alloc;
	DynamicString absPath(alloc);
	getAbsolutePath(path, absPath);

	if (!Os::exists(absPath.toCStr()))
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
