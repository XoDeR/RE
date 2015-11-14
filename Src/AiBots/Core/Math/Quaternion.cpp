// Copyright (c) 2015 Volodymyr Syvochka
#include "Core/Math/Quaternion.h"
#include "Core/Math/MathFn.h"

namespace Rio
{
	const Quaternion Quaternion::Identity = Quaternion(0.0, 0.0, 0.0, 1.0);

	float QuaternionFn::getLength(const Quaternion& q)
	{
		return MathFn::sqrt(q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z);
	}

	Radian QuaternionFn::getAngle(const Quaternion& q)
	{
		return 2.0f * MathFn::acos(q.w);
	}

	Vector3 QuaternionFn::getAxis(const Quaternion& q)
	{
		// 1 - cos(theta/2)*cos(theta/2) = sin(theta/2)*sin(theta/2)
		const float s2 = 1.0f - q.w * q.w;

		if (s2 <= 0.0f)
			return Vector3{ 0, 0, 1 };

		const float invs2 = 1.0f / MathFn::sqrt(s2);

		return q.xyz * invs2;
	}


	Quaternion QuaternionFn::getAngleAxis(const Radian& angle, const Vector3& axis)
	{
		Quaternion q;

		const Vector3 a = getNormalized(axis);

		const float s = MathFn::sin(0.5f * angle);

		q.xyz = a * s;
		q.w = MathFn::cos(0.5f * angle);

		return q;
	}


	Radian QuaternionFn::getRoll(const Quaternion& q)
	{
		return MathFn::atan2(2.0f * q.x * q.y + q.z * q.w,
			q.x * q.x + q.w * q.w - q.y * q.y - q.z * q.z);
	}


	Radian QuaternionFn::getPitch(const Quaternion& q)
	{
		return MathFn::atan2(2.0f * q.y * q.z + q.w * q.x,
			q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z);
	}

	Radian QuaternionFn::getYaw(const Quaternion& q)
	{
		return MathFn::asin(-2.0f * (q.x * q.z - q.w * q.y));
	}


	Quaternion QuaternionFn::slerp(const Quaternion& x, const Quaternion& y, float t)
	{
		RIO_ASSERT(t >= 0.0f && t <= 1.0f, "MathFn::slerp `t` out range (0..1).");

		Quaternion z = y;

		float cosTheta = QuaternionFn::dot(x, y);

		if (cosTheta < 0.0f)
		{
			z = -y;
			cosTheta = -cosTheta;
		}

		if (cosTheta > 1.0f)
		{
			return Quaternion{ MathFn::lerp(x.x, y.x, t),
				MathFn::lerp(x.y, y.y, t),
				MathFn::lerp(x.z, y.z, t),
				MathFn::lerp(x.w, y.w, t) };
		}

		Radian angle = MathFn::acos(cosTheta);

		Quaternion result = MathFn::sin(Radian{ 1.0f } -(t * angle)) * x + MathFn::sin(t * angle) * z;
		return result * (1.0f / MathFn::sin(angle));
	}

	Quaternion QuaternionFn::getPowerOf(const Quaternion& q, float exp)
	{
		if (MathFn::abs(q.w) < 0.9999)
		{
			Quaternion tmp;
			float alpha = acos(q.w); // alpha = theta/2
			float newAlpha = alpha * exp;
			tmp.w = cos(newAlpha);
			float mult = sin(newAlpha) / sin(alpha);
			tmp.x = q.x * mult;
			tmp.y = q.y * mult;
			tmp.z = q.z * mult;
			return tmp;
		}

		return q;
	}

} // namespace Rio
