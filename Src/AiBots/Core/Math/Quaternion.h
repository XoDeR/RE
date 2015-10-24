// Copyright (c) 2015 Volodymyr Syvochka
#pragma once

#include "Core/Base/Config.h"

#include "Core/Base/Types.h"
#include "Core/Math/Vector3.h"
#include "Core/Math/Vector4.h"

namespace Rio
{

	struct Quaternion
	{
		Quaternion();
		Quaternion(float nx, float ny, float nz, float nw);
		Quaternion(const Vector3& axis, float angle);
		float& operator[](uint32_t i);
		const float& operator[](uint32_t i) const;
		Quaternion& operator*=(const Quaternion& a);

		float x, y, z, w;
	};

	inline Quaternion::Quaternion()
	{
		// Do not initialize
	}

	inline Quaternion::Quaternion(float nx, float ny, float nz, float nw)
		: x(nx)
		, y(ny)
		, z(nz)
		, w(nw)
	{
	}

	inline Quaternion::Quaternion(const Vector3& axis, float angle)
		: x(axis.x * sin(angle * 0.5f))
		, y(axis.y * sin(angle * 0.5f))
		, z(axis.z * sin(angle * 0.5f))
		, w(cos(angle * 0.5f))
	{
	}

	inline float& Quaternion::operator[](uint32_t i)
	{
		RIO_ASSERT(i < 4, "Index out of bounds");
		return (&x)[i];
	}

	inline const float& Quaternion::operator[](uint32_t i) const
	{
		RIO_ASSERT(i < 4, "Index out of bounds");
		return (&x)[i];
	}

	inline Quaternion& Quaternion::operator*=(const Quaternion& a)
	{
		const float t_w = w*a.w - x*a.x - y*a.y - z*a.z;
		const float t_x = w*a.x + x*a.w + y*a.z - z*a.y;
		const float t_y = w*a.y + y*a.w + z*a.x - x*a.z;
		const float t_z = w*a.z + z*a.w + x*a.y - y*a.x;

		x = t_x;
		y = t_y;
		z = t_z;
		w = t_w;

		return *this;
	}

	// Negate
	Quaternion operator-(const Quaternion& q);
	// Multiplies the quaternions a and b. (i.e. rotates first by a then by b).
	Quaternion operator*(Quaternion a, const Quaternion& b);
	// Multiplies the quaternion a by the scalar k.
	Quaternion operator*(const Quaternion& a, float k);

	inline Quaternion operator-(const Quaternion& q)
	{
		return Quaternion(-q.x, -q.y, -q.z, -q.w);
	}

	inline Quaternion operator*(Quaternion a, const Quaternion& b)
	{
		a *= b;
		return a;
	}

	inline Quaternion operator*(const Quaternion& a, float k)
	{
		return Quaternion(a.x * k, a.y * k, a.z * k, a.w * k);
	}

	namespace QuaternionFn
	{
		const Quaternion IDENTITY = Quaternion(0.0, 0.0, 0.0, 1.0);
		// Returns the dot product between quaternions a and b.
		float dot(const Quaternion& a, const Quaternion& b);
		// Returns the length of q.
		float getLength(const Quaternion& q);
		// Normalizes the quaternion and returns the result.
		Quaternion& normalize(Quaternion& q);
		// Returns the conjugate of quaternion
		Quaternion getConjugate(const Quaternion& q);
		// Returns the inverse of quaternion
		Quaternion getInverted(const Quaternion& q);
		// Returns the quaternion raised to the power of exp.
		Quaternion getPowerOf(const Quaternion& q, float exp);

		inline float dot(const Quaternion& a, const Quaternion& b)
		{
			return a.w * b.w + a.x * b.x + a.y * b.y + a.z * b.z;
		}

		inline float getLength(const Quaternion& q)
		{
			return sqrt(q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z);
		}

		inline Quaternion& normalize(Quaternion& q)
		{
			const float invLen = 1.0f / getLength(q);
			q.x *= invLen;
			q.y *= invLen;
			q.z *= invLen;
			q.w *= invLen;
			return q;
		}

		inline Quaternion getConjugate(const Quaternion& q)
		{
			return Quaternion(-q.x, -q.y, -q.z, q.w);
		}

		inline Quaternion getInverted(const Quaternion& q)
		{
			return getConjugate(q) * (1.0f / getLength(q));
		}

		inline Quaternion getPowerOf(const Quaternion& q, float exp)
		{
			if (abs(q.w) < 0.9999)
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
	} // namespace QuaternionFn

} // namespace Rio
