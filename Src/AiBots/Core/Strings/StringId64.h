// Copyright (c) 2015 Volodymyr Syvochka
#pragma once

#include "Core/Base/Config.h"
#include "Core/Base/Types.h"

namespace Rio
{

struct StringId64
{
	uint64_t id;

	StringId64() : id(0) {}
	explicit StringId64(uint64_t id) : id(id) {}
	explicit StringId64(const char* str);
	StringId64(const char* str, uint32_t len);
	StringId64 operator=(StringId64 a) { id = a.id; return *this; }
	bool operator==(StringId64 a) const { return id == a.id; }
	bool operator!=(StringId64 a) const { return id != a.id; }
	bool operator<(StringId64 a) const { return id < a.id; }
	uint64_t getId() const { return id; }
};

} // namespace Rio