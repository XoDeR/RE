#include "Core/Math/Vector2.h"
#include "Core/Math/MathFn.h"

namespace Rio
{
	const Vector2 Vector2::Zero = { 0, 0 };

	bool operator==(const Vector2& a, const Vector2& b)
	{
		return MathFn::equals(a.x, b.x) && MathFn::equals(a.y, b.y);
	}


	float Vector2Fn::getLength(const Vector2& a)
	{
		return MathFn::sqrt(a.x * a.x + a.y * a.y);
	}


	Radian Vector2Fn::getAngle(const Vector2& a, const Vector2& b)
	{
		return MathFn::acos(dot(a, b) / (getLength(a) * getLength(b)));
	}

} // namespace Rio
