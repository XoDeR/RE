// Copyright (c) 2015 Volodymyr Syvochka
#pragma once

#include "Core/Base/Config.h"
#include "Core/Base/Types.h"
#include "Core/Debug/Error.h"
#include "Core/Containers/Vector.h"
#include "Core/Strings/DynamicString.h"
#include "Core/Strings/StringUtils.h"
#include "Core/Memory/TempAllocator.h"

namespace Rio 
{
namespace Os
{
	void log(const char* msg);
	bool exists(const char* path);
	// Returns whether the path is a directory.
	bool isDirectory(const char* path);
	// Returns whether the path is a regular file.
	bool isFile(const char* path);
	// Creates a regular file.
	void createFile(const char* path);
	// Deletes a regular file.
	void deleteFile(const char* path);
	void createDirectory(const char* path);
	// Deletes a directory.
	void deleteDirectory(const char* path);
	// Returns the list of files in the directory. Optionally recursively walks into subdirectories
	// Does not follow symbolic links
	void getFileList(const char* path, Vector<DynamicString>& files);
	const char* getCurrentDirName(char* buf, size_t size);
	int64_t getClockTime();
	int64_t getClockFrequency();
	void* openLibrary(const char* path);
	void closeLibrary(void* library);
	void* lookupLibrarySymbol(void* library, const char* name);
	// Executes a process
	// args[0] is the path to the program executable, args[1, 2, ..., n-1] is a list of arguments, args[n] is NULL
	int executeProcess(const char* args[]);
} // namespace Os

} // namespace Rio
