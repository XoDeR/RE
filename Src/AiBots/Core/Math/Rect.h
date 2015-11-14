#pragma once

#include "Core/Base/Config.h"

#include "Core/Base/Types.h"

namespace Rio
{
	struct Vector2;

	struct Rect
	{
		float x;
		float y;
		float width;
		float height;
	};

	bool operator==(const Rect& left, const Rect& right);
	bool operator!=(const Rect& left, const Rect& right);

	bool contains(const Rect& a, const Vector2& point);
	bool intersects(const Rect& a, const Rect& b);
	bool intersects(const Rect& a, const Rect& b, Rect& intersection);

} // namespace Rio
