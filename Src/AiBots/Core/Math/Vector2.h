#pragma once

#include "Core/Base/Config.h"

#include "Core/Debug/Error.h"
#include "Core/Base/Types.h"
#include "Core/Math/MathUtils.h"

namespace Rio
{
	struct Vector2
	{
		Vector2();
		Vector2(float val);
		Vector2(float nx, float ny);
		Vector2(const float v[2]);

		float& operator[](uint32_t i);
		const float& operator[](uint32_t i) const;

		Vector2& operator+=(const Vector2& a);
		Vector2& operator-=(const Vector2& a);
		Vector2& operator*=(float k);
		Vector2& operator/=(float k);

		float x;
		float y;
	};

	inline Vector2::Vector2()
	{
		// Do not initialize
	}

	inline Vector2::Vector2(float val) : x(val), y(val)
	{
	}

	inline Vector2::Vector2(float nx, float ny) : x(nx), y(ny)
	{
	}

	inline Vector2::Vector2(const float a[2]) : x(a[0]), y(a[1])
	{
	}

	inline const float& Vector2::operator[](uint32_t i) const
	{
		RIO_ASSERT(i < 2, "Index out of bounds");
		return (&x)[i];
	}

	inline float& Vector2::operator[](uint32_t i)
	{
		RIO_ASSERT(i < 2, "Index out of bounds");
		return (&x)[i];
	}

	inline Vector2& Vector2::operator+=(const Vector2& a)
	{
		x += a.x;
		y += a.y;

		return *this;
	}

	inline Vector2& Vector2::operator-=(const Vector2& a)
	{
		x -= a.x;
		y -= a.y;

		return *this;
	}

	inline Vector2& Vector2::operator*=(float k)
	{
		x *= k;
		y *= k;

		return *this;
	}

	inline Vector2& Vector2::operator/=(float k)
	{
		RIO_ASSERT(k != (float)0.0, "Division by zero");

		float inv = (float)(1.0 / k);

		x *= inv;
		y *= inv;

		return *this;
	}

	Vector2 operator-(const Vector2& a);
	Vector2 operator+(Vector2 a, const Vector2& b);
	Vector2 operator-(Vector2 a, const Vector2& b);
	Vector2 operator*(Vector2 a, float k);
	Vector2 operator*(float k, Vector2 a);

	inline Vector2 operator-(const Vector2& a)
	{
		return Vector2(-a.x, -a.y);
	}

	inline Vector2 operator+(Vector2 a, const Vector2& b)
	{
		a += b;
		return a;
	}

	inline Vector2 operator-(Vector2 a, const Vector2& b)
	{
		a -= b;
		return a;
	}

	inline Vector2 operator*(Vector2 a, float k)
	{
		a *= k;
		return a;
	}

	inline Vector2 operator*(float k, Vector2 a)
	{
		a *= k;
		return a;
	}

	inline Vector2 operator/(Vector2 a, float k)
	{
		a /= k;
		return a;
	}

	inline bool operator==(const Vector2& a, const Vector2& b)
	{
		return equals(a.x, b.x) && equals(a.y, b.y);
	}

	// Divides the vector by the scalar and returns the result.
	Vector2 operator/(Vector2 a, float k);

	// Returns true whether the vectors are equal.
	bool operator==(const Vector2& a, const Vector2& b);

	namespace Vector2Fn
	{
		const Vector2 ZERO = Vector2(0, 0);
		float dot(const Vector2& a, const Vector2& b);
		float getLength(const Vector2& a);
		float getLengthSquared(const Vector2& a);
		void setLength(Vector2& a, float len);
		// Normalizes and returns the result.
		Vector2 normalize(Vector2& a);
		// Returns the distance between the points
		float getDistance(const Vector2& a, const Vector2& b);
		// Returns the angle between the vectors
		float getAngle(const Vector2& a, const Vector2& b);
		// Returns the pointer to the data
		float* toFloatPtr(Vector2& a);
		const float* toFloatPtr(const Vector2& a);
		
		inline float dot(const Vector2& a, const Vector2& b)
		{
			return a.x * b.x + a.y * b.y;
		}
		
		inline float getLength(const Vector2& a)
		{
			return sqrt(a.x * a.x + a.y * a.y);
		}
		
		inline float getLengthSquared(const Vector2& a)
		{
			return a.x * a.x + a.y * a.y;
		}
		
		inline void setLength(Vector2& a, float len)
		{
			normalize(a);

			a.x *= len;
			a.y *= len;
		}

		inline Vector2 normalize(Vector2& a)
		{
			float invertedLength = 1.0f / getLength(a);

			a.x *= invertedLength;
			a.y *= invertedLength;

			return a;
		}

		inline float getDistance(const Vector2& a, const Vector2& b)
		{
			return getLength(b - a);
		}

		inline float getAngle(const Vector2& a, const Vector2& b)
		{
			return acos(dot(a, b) / (getLength(a) * getLength(b)));
		}

		inline float* toFloatPtr(Vector2& a)
		{
			return &a.x;
		}

		inline const float* toFloatPtr(const Vector2& a)
		{
			return &a.x;
		}
	} // namespace Vector2Fn

} // namespace Rio
