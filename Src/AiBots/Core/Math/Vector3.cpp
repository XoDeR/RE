#include "Core/Math/Vector3.h"
#include "Core/Math/MathFn.h"

namespace Rio
{
	// static
	const Vector3 Vector3::Zero = { 0, 0, 0 };
	const Vector3 Vector3::Xaxis = Vector3(1, 0, 0);
	const Vector3 Vector3::Yaxis = Vector3(0, 1, 0);
	const Vector3 Vector3::Zaxis = Vector3(0, 0, 1);
	const Vector3 Vector3::FORWARD = Vector3(0, 0, 1);
	const Vector3 Vector3::BACKWARD = Vector3(0, 0, -1);
	const Vector3 Vector3::LEFT = Vector3(-1, 0, 0);
	const Vector3 Vector3::RIGHT = Vector3(1, 0, 0);
	const Vector3 Vector3::UP = Vector3(0, 1, 0);
	const Vector3 Vector3::DOWN = Vector3(0, -1, 0);

	Vector3 getNormalized(const Vector3& a)
	{
		return a * (1.0f / Vector3Fn::getLength(a));
	}

	bool operator==(const Vector3& a, const Vector3& b)
	{
		return MathFn::equals(a.x, b.x) && MathFn::equals(a.y, b.y) && MathFn::equals(a.z, b.z);
	}



	float Vector3Fn::getLength(const Vector3& a)
	{
		return MathFn::sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
	}


	Radian Vector3Fn::getAngle(const Vector3& a, const Vector3& b)
	{
		return MathFn::acos(dot(a, b) / (getLength(a) * getLength(b)));
	}

} // namespace Rio
