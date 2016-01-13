// Copyright (c) 2015 Volodymyr Syvochka
#pragma once

#include "Core/Base/Config.h"

#include "Core/FileSystem/File.h"
#include "Core/Strings/DynamicString.h"
#include "Core/Containers/Vector.h"

namespace Rio
{
	// Provides a platform-independent way to access files and directories
	//
	// Accessing files:
	// Every file and every directory must be accessed through the FileSystem.
	// Not a single C/C++ std file io call or other similar facilities
	// should be used in any other part of the engine in order to maintain
	// absolute platform independence.
	//
	// Filesystem maintains a root path which acts as base directory for every
	// file operation; access to files outside the root path is not allowed. If
	// you really need it, instantiate another filesystem whith the appropriate
	// root path (e.g.)
	//
	// Filesystem fs("/home/foo"); // fs will use "/home/foo" as root path
	//
	// fs.isFile("bar.txt");      // file "bar.txt" is relative to the root path,
	//                             // so it refers to "/home/foo/bar.txt"
	//
	// The filesystem will take care of the necessary path conversions.
	// The root path must be an absolute path for the underlying operating system.
	// Examples of valid root paths:
	//
	// 1) "/home/foo"
	// 2) "C:\Users\Mike"
	//
	// The relative paths, used to access files, must follow some strict rules:
	//
	// a) Only unix-like pathnames (i.e. case sensitive and using '/' as separator)
	//    are allowed.
	// b) Only relative paths are allowed: the filesystem is responsible for
	//    the creation of its absolute platform-specific counterpart.
	// c) Filesystem forbids pathnames containing '.' and '..' to prevent access to
	//    files outside the filesystem's root path.
	// d) Platform specific characters like '/', '\\' and ':' are forbidden as well.
	// e) Symlinks, on platforms which support them, are NOT resolved for the same
	//    reason of c)
	// f) Although mixed-case pathnames are allowed, it is generally safer to use
	//    only lower-case ones for maximum compatibility.
	//
	// Examples of valid relative paths.
	//
	// 1) data/fonts/arial.font
	// 2) arial.font
	// 3) foo/bar
	class Filesystem
	{
	public:
		Filesystem() {};
		virtual ~Filesystem() {};
	private:
		// Disable copying
		Filesystem(const Filesystem&) = delete;
		Filesystem& operator=(const Filesystem&) = delete;
	public:
		virtual File* fileOpen(const char* path, FileOpenMode mode) = 0;
		virtual void fileClose(File& file) = 0;
		virtual bool doesPathExist(const char* path) = 0;
		// Returns true if path is a directory.
		virtual bool isDirectory(const char* path) = 0;
		// Returns true if path is a regular file.
		virtual bool isFile(const char* path) = 0;
		virtual uint64_t getLastModifiedTime(const char* path) = 0;
		virtual void createDirectory(const char* path) = 0;
		virtual void deleteDirectory(const char* path) = 0;
		virtual void createFile(const char* path) = 0;
		virtual void deleteFile(const char* path) = 0;
		// Returns the relative file names in the given path.
		virtual void getFileList(const char* path, Vector<DynamicString>& files) = 0;
		// Returns the absolute path of the given path based on
		// the root path of the file source. If the path is absolute, the given path is returned.
		virtual void getAbsolutePath(const char* path, DynamicString& osPath) = 0;
	};

} // namespace Rio
