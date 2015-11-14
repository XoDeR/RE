#include "Core/Math/Rect.h"

#include "Core/Math/Vector2.h"
#include "Core/Math/MathFn.h"

namespace Rio
{
	bool contains(const Rect& a, const Vector2& point)
	{
		// TODO: compare similar due to floating point arithmetic
		const float minX = MathFn::min(a.x, a.x + a.width);
		const float maxX = MathFn::max(a.x, a.x + a.width);
		const float minY = MathFn::min(a.y, a.y + a.height);
		const float maxY = MathFn::max(a.y, a.y + a.height);

		return (point.x >= minX) && (point.x < maxX) && (point.y >= minY) &&
			(point.y < maxY);
	}

	bool intersects(const Rect& a, const Rect& b)
	{
		Rect intersection;
		return intersects(a, b, intersection);
	}

	bool intersects(const Rect& a, const Rect& b, Rect& intersection)
	{
		const float r1MinX = MathFn::min(a.x, a.x + a.width);
		const float r1MaxX = MathFn::max(a.x, a.x + a.width);
		const float r1MinY = MathFn::min(a.y, a.y + a.height);
		const float r1MaxY = MathFn::max(a.y, a.y + a.height);

		const float r2MinX = MathFn::min(b.x, b.x + b.width);
		const float r2MaxX = MathFn::max(b.x, b.x + b.width);
		const float r2MinY = MathFn::min(b.y, b.y + b.height);
		const float r2MaxY = MathFn::max(b.y, b.y + b.height);

		const float x1 = MathFn::max(r1MinX, r2MinX);
		const float x2 = MathFn::min(r1MaxX, r2MaxX);

		const float y1 = MathFn::max(r1MinY, r2MinY);
		const float y2 = MathFn::min(r1MaxY, r2MaxY);

		// If intersection is valid (positive non-zero area)
		if ((x1 < x2) && (y1 < y2))
		{
			intersection.x = x1;
			intersection.y = y1;
			intersection.width = x2 - x1;
			intersection.height = y2 - y1;
			return true;
		}
		else
		{
			intersection.x = 0;
			intersection.y = 0;
			intersection.width = 0;
			intersection.height = 0;

			return false;
		}
	}

	bool operator==(const Rect& left, const Rect& right)
	{
		// TODO: compare similar due to floating point arithmetic
		return MathFn::equals(left.x, right.x) 
			&& MathFn::equals(left.y, right.y) 
			&& MathFn::equals(left.width, right.width) 
			&& MathFn::equals(left.height, right.height);
	}

	bool operator!=(const Rect& left, const Rect& right)
	{
		return !(left == right);
	}

} // namespace Rio
