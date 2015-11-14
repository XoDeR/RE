// Copyright (c) 2015 Volodymyr Syvochka
#include "Core/Math/Vector4.h"
#include "Core/Math/MathFn.h"

namespace Rio
{
	const Vector4 Vector4::Zero = { 0, 0, 0, 0 };
	const Vector4 Vector4::Xaxis = Vector4(1, 0, 0, 0);
	const Vector4 Vector4::Yaxis = Vector4(0, 1, 0, 0);
	const Vector4 Vector4::Zaxis = Vector4(0, 0, 1, 0);
	const Vector4 Vector4::Waxis = Vector4(0, 0, 0, 1);


	float Vector4Fn::getLength(const Vector4& a)
	{
		return MathFn::sqrt(a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w);
	}

	bool operator==(const Vector4& a, const Vector4& b)
	{
		return MathFn::equals(a.x, b.x) && MathFn::equals(a.y, b.y) && MathFn::equals(a.z, b.z) && MathFn::equals(a.w, b.w);
	}

	Radian Vector4Fn::getAngle(const Vector4& a, const Vector4& b)
	{
		return MathFn::acos(dot(a, b) / (getLength(a) * getLength(b)));
	}

} // namespace Rio
