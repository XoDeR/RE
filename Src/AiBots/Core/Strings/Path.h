// Copyright (c) 2015 Volodymyr Syvochka
#pragma once

#include "Core/Base/Config.h"
#include "Core/Base/Platform.h"

#include "Core/Strings/DynamicString.h"

namespace Rio
{
	// Functions for operating with strings as file paths.
	namespace PathFn
	{
#if RIO_PLATFORM_POSIX
		const char SEPARATOR = '/';
#elif RIO_PLATFORM_WINDOWS
		const char SEPARATOR = '\\';
#endif // RIO_PLATFORM_POSIX

		// Returns whether the path is absolute.
		bool isAbsolutePath(const char* path);
		// Returns whether the path is the root path.
		bool isRootPath(const char* path);
		// Appends path p2 to p1 and fills path with the result.
		void joinPaths(const char* a, const char* b, DynamicString& path);
		// Returns OS-dependent path from OS-independent path
		const char* normalizePath(const char* path);
		// Returns the pathname of the path.
		// "/home/project/shader.hlsl" -> "/home/project"
		// "/home/project" -> "/home"
		// "/home" -> "/"
		// "home" -> ""
		// The path must be valid.
		void getPathNameFromPath(const char* path, char* str, size_t len);
		// Returns the filename of the path.
		// "/home/project/shader.hlsl" -> "shader.hlsl"
		// "/home/project/shader" -> "shader"
		// "/home -> "home"
		// "/" -> ""
		// The path must be valid.
		void getFileNameFromPath(const char* path, char* str, size_t len);
		


		// Returns the basename of the path.
		// "/home/project/texture.tga" -> "texture.tga"
		// "/home/project" -> "project"
		// "/" -> ""
		const char* getBaseNameFromPath(const char* path);

		// Returns the extension of the path or NULL.
		// "/home/texture.tga" -> "tga"
		// "/home/texture" -> NULL
		const char* getExtensionFromPath(const char* path);

		// Returns the basename of the path.
		// "/home/project/shader.hlsl" -> "shader"
		// "/home/project" -> "project"
		// "/" -> ""
		// The path must be valid.
		void getBaseNameFromPath(const char* path, char* str, size_t len);
		
		// Returns the extension of the path.
		// "/home/project/shader.hlsl" -> "hlsl"
		// "/home/project.x/shader" -> ""
		// The path must be valid.
		void getExtensionFromPath(const char* path, char* str, size_t len);
		
		// Returns the filename without the extension.
		// "/home/project/shader.hlsl" -> "/home/project/shader"
		// "/home/project/shader" -> "/home/project/shader"
		// The path must be valid.
		void getFileNameWithoutExtensionFromPath(const char* path, char* str, size_t len);
		
		//bool segments(const char* path, Array<Str>& ret);
		// Fills ret with the same path but without the trailing directory separator.
		// "/home/project/shader.hlsl/" -> "/home/project/shader.hlsl"
		// "/home/project/shader.hlsl" -> "/home/project/shader.hlsl"
		// The path must be valid.
		void stripTrailingSeparator(const char* path, char* ret, size_t len);
	} // namespace PathFn

} // namespace Rio
