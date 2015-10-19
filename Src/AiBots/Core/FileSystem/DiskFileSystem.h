// Copyright (c) 2015 Volodymyr Syvochka
#pragma once

#include "Core/Base/Config.h"

#include "Core/FileSystem/FileSystem.h"

namespace Rio 
{

// Access files on disk.
// All the file paths can be either relative or absolute.
// When a relative path is given, it is automatically translated
// to its absolute counterpart based on the file source's root path.
// Accessing files using absolute path directly is also possible,
// but platform-specific and thus generally not recommended.
class DiskFilesystem : public Filesystem
{
public:
	// Sets the root path to the current working directory of
	// the engine executable.
	DiskFilesystem();
	// Sets the root path to the given prefix.
	// The prefix must be absolute.
	DiskFilesystem(const char* prefix);
	// Opens the file at the given path with the given mode.
	File* open(const char* path, FileOpenMode mode);
	// Closes the given file.
	void close(File* file);
	// Returns whether path exists.
	bool exists(const char* path);
	// Returns true if path is a directory.
	bool isDirectory(const char* path);
	// Returns true if path is a regular file.
	bool isFile(const char* path);
	void createDirectory(const char* path);
	void deleteDirectory(const char* path);
	void createFile(const char* path);
	void deleteFile(const char* path);
	// Returns the relative file names in the given path.
	void getFileList(const char* path, Vector<DynamicString>& files);
	// Returns the absolute path of the given path based on
	// the root path of the file source. If path is absolute,
	// the given path is returned.
	void getAbsolutePath(const char* path, DynamicString& osPath);
private:
	DynamicString fileSystemPrefix;
};

} // namespace Rio
