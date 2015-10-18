// Copyright (c) 2015 Volodymyr Syvochka
#include "Core/Os/Os.h"

#if RIO_PLATFORM_WINDOWS
#include "Core/Os/Windows/Headers_Windows.h"
#include <io.h>
#endif // RIO_PLATFORM_WINDOWS

namespace Rio
{
	namespace Os
	{
#if RIO_PLATFORM_WINDOWS
		void log(const char* msg)
		{
			puts(msg);
			fflush(stdout);
		}

		bool exists(const char* path)
		{
			return _access(path, 0) != -1;
		}

		bool isDirectory(const char* path)
		{
			DWORD fAttr = GetFileAttributes(path);
			return (fAttr != INVALID_FILE_ATTRIBUTES && (fAttr & FILE_ATTRIBUTE_DIRECTORY) != 0);
		}

		bool isFile(const char* path)
		{
			DWORD fAttr = GetFileAttributes(path);
			return (fAttr != INVALID_FILE_ATTRIBUTES && (fAttr & FILE_ATTRIBUTE_DIRECTORY) == 0);
		}

		void createFile(const char* path)
		{
			HANDLE hFile = CreateFile(path, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			RIO_ASSERT(hFile != INVALID_HANDLE_VALUE, "CreateFile: GetLastError = %d", GetLastError());
			CloseHandle(hFile);
		}

		// Deletes a regular file.
		void deleteFile(const char* path)
		{
			BOOL err = DeleteFile(path);
			RIO_ASSERT(err != 0, "DeleteFile: GetLastError = %d", GetLastError());
			RIO_UNUSED(err);
		}

		void createDirectory(const char* path)
		{
			BOOL err = CreateDirectory(path, NULL);
			RIO_ASSERT(err != 0, "CreateDirectory: GetLastError = %d", GetLastError());
			RIO_UNUSED(err);
		}

		// Deletes a directory.
		void deleteDirectory(const char* path)
		{
			BOOL err = RemoveDirectory(path);
			RIO_ASSERT(err != 0, "RemoveDirectory: GetLastError = %d", GetLastError());
			RIO_UNUSED(err);
		}

		void getFileList(const char* path, Vector<DynamicString>& files)
		{
			HANDLE file = INVALID_HANDLE_VALUE;
			WIN32_FIND_DATA ffd;

			char curPath[1024];

			strnCpy(curPath, path, strLen(path) + 1);
			strnCat(curPath, "\\*", 2);

			file = FindFirstFile(curPath, &ffd);

			do
			{
				RIO_ASSERT(file != INVALID_HANDLE_VALUE, "Unable to list files. errono %d", GetLastError());

				if ((strCmp(ffd.cFileName, ".") == 0) || (strCmp(ffd.cFileName, "..") == 0))
				{
					continue;
				}

				DynamicString filename(getDefaultAllocator());

				filename = ffd.cFileName;
				VectorFn::pushBack(files, filename);
			} while (FindNextFile(file, &ffd) != 0);

			FindClose(file);
		}

		const char* getCurrentDirName(char* buf, size_t size)
		{
			GetCurrentDirectory(size, buf);
			return buf;
		}

		int64_t getClockTime()
		{
			LARGE_INTEGER ttime;
			QueryPerformanceCounter(&ttime);
			return (int64_t)ttime.QuadPart;
		}

		int64_t getClockFrequency()
		{
			LARGE_INTEGER freq;
			QueryPerformanceFrequency(&freq);
			return (int64_t)freq.QuadPart;
		}

		void* openLibrary(const char* path)
		{
			return (void*)LoadLibraryA(path);
		}

		void closeLibrary(void* library)
		{
			FreeLibrary((HMODULE)library);
		}

		void* lookupLibrarySymbol(void* library, const char* name)
		{
			return (void*)GetProcAddress((HMODULE)library, name);
		}

		int executeProcess(const char* args[])
		{
			STARTUPINFO info;
			memset(&info, 0, sizeof(info));
			info.cb = sizeof(info);

			PROCESS_INFORMATION process;
			memset(&process, 0, sizeof(process));

			TempAllocator1024 alloc;
			DynamicString cmds(alloc);

			for (uint32_t i = 0; args[i] != NULL; i++)
			{
				cmds += args[i];
				cmds += ' ';
			}

			int err = CreateProcess(args[0], (char*)cmds.toCStr(), NULL, NULL, TRUE, 0, NULL, NULL, &info, &process);
			RIO_ASSERT(err != 0, "CreateProcess: GetLastError = %d", GetLastError());
			RIO_UNUSED(err);

			DWORD exitcode = 1;
			::WaitForSingleObject(process.hProcess, INFINITE);
			GetExitCodeProcess(process.hProcess, &exitcode);
			CloseHandle(process.hProcess);
			CloseHandle(process.hThread);
			return (int)exitcode;
		}
#endif // RIO_PLATFORM_WINDOWS
	} // namespace Os
} // namespace Rio

