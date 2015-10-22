#pragma once

#include "Core/Base/Config.h"

#include "Core/Debug/Error.h"
#include "Core/Base/Types.h"
#include "Core/Math/MathUtils.h"
#include "Core/Math/Vector2.h"

namespace Rio
{
	struct Vector3
	{
		Vector3();
		Vector3(float val);
		Vector3(float nx, float ny, float nz);
		Vector3(const float v[3]);

		float& operator[](uint32_t i);
		const float& operator[](uint32_t i) const;

		Vector3& operator+=(const Vector3& a);
		Vector3& operator-=(const Vector3& a);
		Vector3& operator*=(float k);
		Vector3& operator/=(float k);

		float x, y, z;
	};

	inline Vector3::Vector3()
	{
		// Do not initialize
	}

	inline Vector3::Vector3(float val) : x(val), y(val), z(val)
	{
	}

	inline Vector3::Vector3(float nx, float ny, float nz) : x(nx), y(ny), z(nz)
	{
	}

	inline Vector3::Vector3(const float v[3]) : x(v[0]), y(v[1]), z(v[2])
	{
	}

	inline const float& Vector3::operator[](uint32_t i) const
	{
		RIO_ASSERT(i < 3, "Index out of bounds");

		return (&x)[i];
	}

	inline float& Vector3::operator[](uint32_t i)
	{
		RIO_ASSERT(i < 3, "Index out of bounds");

		return (&x)[i];
	}

	inline Vector3& Vector3::operator+=(const Vector3& a)
	{
		x += a.x;
		y += a.y;
		z += a.z;

		return *this;
	}

	inline Vector3& Vector3::operator-=(const Vector3& a)
	{
		x -= a.x;
		y -= a.y;
		z -= a.z;

		return *this;
	}

	inline Vector3& Vector3::operator*=(float k)
	{
		x *= k;
		y *= k;
		z *= k;

		return *this;
	}

	inline Vector3& Vector3::operator/=(float k)
	{
		RIO_ASSERT(k != (float)0.0, "Division by zero");

		float inv = (float)(1.0 / k);

		x *= inv;
		y *= inv;
		z *= inv;

		return *this;
	}

	// Negate
	Vector3 operator-(const Vector3& a);
	// Adds the vector a to b and returns the result.
	Vector3 operator+(Vector3 a, const Vector3& b);
	// Subtracts the vector b from a and returns the result.
	Vector3 operator-(Vector3 a, const Vector3& b);
	// Multiplies the vector a by the scalar k and returns the result.
	Vector3 operator*(Vector3 a, float k);
	Vector3 operator*(float k, Vector3 a);
	// Divides the vector a by the scalar k and returns the result.
	Vector3 operator/(Vector3 a, float k);

	// Returns true whether the vectors a and b are equal.
	bool operator==(const Vector3& a, const Vector3& b);

	inline Vector3 operator-(const Vector3& a)
	{
		return Vector3(-a.x, -a.y, -a.z);
	}

	inline Vector3 operator+(Vector3 a, const Vector3& b)
	{
		a += b;
		return a;
	}

	inline Vector3 operator-(Vector3 a, const Vector3& b)
	{
		a -= b;
		return a;
	}

	inline Vector3 operator*(Vector3 a, float k)
	{
		a *= k;
		return a;
	}

	inline Vector3 operator*(float k, Vector3 a)
	{
		a *= k;
		return a;
	}

	inline Vector3 operator/(Vector3 a, float k)
	{
		a /= k;
		return a;
	}

	inline bool operator==(const Vector3& a, const Vector3& b)
	{
		return equals(a.x, b.x) && equals(a.y, b.y) && equals(a.z, b.z);
	}

	namespace Vector3Fn
	{
		const Vector3 ZERO = Vector3(0, 0, 0);
		const Vector3 XAXIS = Vector3(1, 0, 0);
		const Vector3 YAXIS = Vector3(0, 1, 0);
		const Vector3 ZAXIS = Vector3(0, 0, 1);
		const Vector3 FORWARD = Vector3(0, 0, 1);
		const Vector3 BACKWARD = Vector3(0, 0, -1);
		const Vector3 LEFT = Vector3(-1, 0, 0);
		const Vector3 RIGHT = Vector3(1, 0, 0);
		const Vector3 UP = Vector3(0, 1, 0);
		const Vector3 DOWN = Vector3(0, -1, 0);
		float dot(const Vector3& a, const Vector3& b);
		Vector3 cross(const Vector3& a, const Vector3& b);
		float getLength(const Vector3& a);
		float getLengthSquared(const Vector3& a);
		void setLength(Vector3& a, float len);
		Vector3 normalize(Vector3& a);
		float getDistance(const Vector3& a, const Vector3& b);
		float getAngle(const Vector3& a, const Vector3& b);
		float* toFloatPtr(Vector3& a);
		const float* toFloatPtr(const Vector3& a);
		// Returns the Vector2 portion (truncates z)
		Vector2 toVector2(const Vector3& a);

		inline float dot(const Vector3& a, const Vector3& b)
		{
			return a.x * b.x + a.y * b.y + a.z * b.z;
		}

		inline Vector3 cross(const Vector3& a, const Vector3& b)
		{
			return Vector3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
		}

		inline float getLength(const Vector3& a)
		{
			return sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
		}

		inline float getLengthSquared(const Vector3& a)
		{
			return a.x * a.x + a.y * a.y + a.z * a.z;
		}

		inline void setLength(Vector3& a, float len)
		{
			normalize(a);
			a.x *= len;
			a.y *= len;
			a.z *= len;
		}

		inline Vector3 normalize(Vector3& a)
		{
			float inv_len = 1.0f / getLength(a);

			a.x *= inv_len;
			a.y *= inv_len;
			a.z *= inv_len;

			return a;
		}

		inline float getDistance(const Vector3& a, const Vector3& b)
		{
			return getLength(b - a);
		}

		inline float getAngle(const Vector3& a, const Vector3& b)
		{
			return acos(dot(a, b) / (getLength(a) * getLength(b)));
		}

		inline float* toFloatPtr(Vector3& a)
		{
			return &a.x;
		}

		inline const float* toFloatPtr(const Vector3& a)
		{
			return &a.x;
		}

		inline Vector2 toVector2(const Vector3& a)
		{
			return Vector2(a.x, a.y);
		}
	} // namespace Vector3Fn

} // namespace Rio
