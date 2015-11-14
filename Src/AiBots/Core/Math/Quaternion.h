// Copyright (c) 2015 Volodymyr Syvochka
#pragma once

#include "Core/Base/Config.h"

#include "Core/Base/Types.h"

#include "Core/Math/Angle.h"

#include "Core/Math/Vector3.h"
#include "Core/Math/Vector4.h"

namespace Rio
{
	// Auxiliary
	struct EulerAngles
	{
		Radian pitch;
		Radian yaw;
		Radian roll;
	};

	struct Quaternion
	{
		Quaternion();
		Quaternion(float nx, float ny, float nz, float nw);
		Quaternion(const Vector3& axis, float angle);
		float& operator[](size_t i);
		const float& operator[](size_t i) const;
		Quaternion& operator*=(const Quaternion& a);

		union
		{
			struct
			{
				float x;
				float y;
				float z;
				float w;
			};
			struct
			{
				Vector3 xyz;
			};
			float data[4];
		};

		static const Quaternion Identity; // = Quaternion(0.0, 0.0, 0.0, 1.0);
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

	inline float& Quaternion::operator[](size_t i)
	{
		RIO_ASSERT(i < 4, "Index out of bounds");
		return data[i];
	}

	inline const float& Quaternion::operator[](size_t i) const
	{
		RIO_ASSERT(i < 4, "Index out of bounds");
		return data[i];
	}

	// q = (v, s) -> where v = Vector3, s = Scalar (float)
	// (v1, s1)(v2, s2) = (s1v2 + s2v1 + v1 x v2, s1s2 - v1 . v2)
	// . -> dot product
	// x -> cross product
	inline Quaternion& Quaternion::operator*=(const Quaternion& a)
	{
		const float tW = w*a.w - x*a.x - y*a.y - z*a.z;
		const float tX = w*a.x + x*a.w + y*a.z - z*a.y;
		const float tY = w*a.y + y*a.w + z*a.x - x*a.z;
		const float tZ = w*a.z + z*a.w + x*a.y - y*a.x;

		x = tX;
		y = tY;
		z = tZ;
		w = tW;

		return *this;
	}

	// Negate
	Quaternion operator-(const Quaternion& q);
	// Multiplies the quaternions a and b. (i.e. rotates first by a then by b).
	Quaternion operator*(Quaternion a, const Quaternion& b);
	// Multiplies the quaternion a by the scalar k.
	Quaternion operator*(const Quaternion& a, float k);
	// Rotate v by q
	Vector3 operator*(const Quaternion& q, const Vector3& v);
	Quaternion operator+(const Quaternion& a, const Quaternion& b);
	Quaternion operator-(const Quaternion& a, const Quaternion& b);
	Quaternion operator/(const Quaternion& a, float s);
	bool operator==(const Quaternion& a, const Quaternion& b);
	bool operator!=(const Quaternion& a, const Quaternion& b);

	inline Quaternion operator*(float s, const Quaternion& q) { return q * s; }

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

	inline Vector3 operator*(const Quaternion& q, const Vector3& v)
	{
		// return q * Quaternion(v, 0) * getConjugate(q); // Another way, more expensive
		const Vector3 t = 2.0f * Vector3Fn::cross(q.xyz, v);
		return (v + q.w * t + Vector3Fn::cross(q.xyz, t));
	}

	inline Quaternion operator+(const Quaternion& a, const Quaternion& b)
	{
		Quaternion c;

		c.x = a.x + b.x;
		c.y = a.y + b.y;
		c.z = a.z + b.z;
		c.w = a.w + b.w;

		return c;
	}

	inline Quaternion operator-(const Quaternion& a, const Quaternion& b)
	{
		Quaternion c;

		c.x = a.x - b.x;
		c.y = a.y - b.y;
		c.z = a.z - b.z;
		c.w = a.w - b.w;

		return c;
	}


	inline Quaternion operator/(const Quaternion& a, float s)
	{
		Quaternion c = a;

		c.x /= s;
		c.y /= s;
		c.z /= s;
		c.w /= s;

		return c;
	}

	inline bool operator==(const Quaternion& a, const Quaternion& b)
	{
		for (size_t i = 0; i < 4; i++)
		{
			if (a.data[i] != b.data[i])
			{
				return false;
			}
		}
		return true;
	}

	inline bool operator!=(const Quaternion& a, const Quaternion& b)
	{
		return !operator==(a, b);
	}

	namespace QuaternionFn
	{
		float dot(const Quaternion& a, const Quaternion& b);
		Quaternion cross(const Quaternion& a, const Quaternion& b);
		float getLength(const Quaternion& q);
		float getLengthSquared(const Quaternion& q);
		// Normalizes the quaternion and returns the result.
		Quaternion& normalize(Quaternion& q);
		Quaternion getNormalized(const Quaternion& q);
		// Returns the getConjugate of quaternion
		Quaternion getConjugate(const Quaternion& q);
		// Returns the inverse of quaternion
		Quaternion getInverted(const Quaternion& q);
		// Returns the quaternion raised to the power of exp.
		Quaternion getPowerOf(const Quaternion& q, float exp);
		Radian getAngle(const Quaternion& q);
		Vector3 getAxis(const Quaternion& q);
		Quaternion getAngleAxis(const Radian& angle, const Vector3& axis);
		Radian getRoll(const Quaternion& q);
		Radian getPitch(const Quaternion& q);
		Radian getYaw(const Quaternion& q);

		EulerAngles quaternionToEulerAngles(const Quaternion& q);
		Quaternion eulerAnglesToQuaternion(const EulerAngles& e,
			const Vector3& xAxis = { 1, 0, 0 },
			const Vector3& yAxis = { 0, 1, 0 },
			const Vector3& zAxis = { 0, 0, 1 });
		inline Quaternion eulerAnglesToQuaternion(const Radian& pitch,
			const Radian& yaw,
			const Radian& roll,
			const Vector3& xAxis = { 1, 0, 0 },
			const Vector3& yAxis = { 0, 1, 0 },
			const Vector3& zAxis = { 0, 0, 1 })
		{
			return eulerAnglesToQuaternion({ pitch, yaw, roll }, xAxis, yAxis, zAxis);
		}

		//*****************
		// Interpolation
		//*****************
		// Spherical Linear Interpolation
		Quaternion slerp(const Quaternion& x, const Quaternion& y, float t);

		// Shoemake's Quaternion Curves
		// Spherical Cubic Interpolation
		inline Quaternion squad(const Quaternion& p,
			const Quaternion& a,
			const Quaternion& b,
			const Quaternion& q,
			float t)
		{
			return slerp(slerp(p, q, t), slerp(a, b, t), 2.0f * t * (1.0f - t));
		}

		inline float dot(const Quaternion& a, const Quaternion& b)
		{
			return a.w * b.w + a.x * b.x + a.y * b.y + a.z * b.z;
		}

		inline Quaternion cross(const Quaternion& a, const Quaternion& b)
		{
			return Quaternion{ a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y,
				a.w * b.y + a.y * b.w + a.z * b.x - a.x * b.z,
				a.w * b.z + a.z * b.w + a.x * b.y - a.y * b.x,
				a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z };
		}

		inline float getLengthSquared(const Quaternion& q)
		{ 
			return dot(q, q); 
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

		inline Quaternion getNormalized(const Quaternion& q)
		{ 
			return q * (1.0f / getLength(q)); 
		}

		inline Quaternion getConjugate(const Quaternion& q)
		{
			return Quaternion(-q.x, -q.y, -q.z, q.w);
		}

		inline Quaternion getInverted(const Quaternion& q)
		{
			return getConjugate(q) * (1.0f / getLength(q));
		}

		Quaternion getPowerOf(const Quaternion& q, float exp);

		inline EulerAngles quaternionToEulerAngles(const Quaternion& q)
		{
			return{ getPitch(q), getYaw(q), getRoll(q) };
		}

		inline Quaternion eulerAnglesToQuaternion(const EulerAngles& e,
			const Vector3& xAxis,
			const Vector3& yAxis,
			const Vector3& zAxis)
		{
			const Quaternion p = getAngleAxis(e.pitch, xAxis);
			const Quaternion y = getAngleAxis(e.pitch, yAxis);
			const Quaternion r = getAngleAxis(e.pitch, zAxis);

			return y * p * r;
		}
	} // namespace QuaternionFn

} // namespace Rio
