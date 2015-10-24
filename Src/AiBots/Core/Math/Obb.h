// Copyright (c) 2015 Volodymyr Syvochka
#pragma once

#include "Core/Base/Config.h"

#include "Core/Math/Matrix4x4.h"
#include "Core/Math/Aabb.h"

namespace Rio
{
	struct Obb
	{
		Matrix4x4 tm;
		Aabb aabb;
	};
} // namespace Rio