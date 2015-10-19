// Copyright (c) 2015 Volodymyr Syvochka
#pragma once

#include "Core/Base/Config.h"

#include "Core/Base/Types.h"
#include "File.h"

namespace Rio
{
	// A reader that offers a convenient way to read from a File
	class FileBinaryReader
	{
	public:
		FileBinaryReader(File& file) : file(file) {}
		size_t read(void* data, size_t size)
		{
			return file.read(data, size);
		}

		template <typename T>
		size_t read(T& data)
		{
			return file.read(&data, sizeof(T));
		}
		void skip(size_t bytes)
		{
			file.skip(bytes);
		}
	private:
		File& file;
	};
} // namespace Rio