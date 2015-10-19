// Copyright (c) 2015 Volodymyr Syvochka
#pragma once

#include "Core/Base/Config.h"

#include "Core/Base/Types.h"
#include "File.h"

namespace Rio
{
	// A writer that offers a convenient way to write to a File
	class FileBinaryWriter
	{
	public:
		FileBinaryWriter(File& file) : file(file) {}

		void write(const void* data, size_t size)
		{
			file.write(data, size);
		}

		template <typename T>
		void write(const T& data)
		{
			file.write(&data, sizeof(T));
		}

		void skip(size_t bytes)
		{
			file.skip(bytes);
		}
	private:
		File& file;
	};
} // namespace Rio