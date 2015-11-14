#pragma once

#include "Core/Base/Config.h"

#include "Core/Debug/Error.h"
#include "Core/Base/Types.h"

#include "Core/Math/Angle.h"

namespace Rio
{
	struct Vector2
	{
		Vector2();
		Vector2(float val);
		Vector2(float nx, float ny);
		Vector2(const float v[2]);

		float& operator[](size_t i);
		const float& operator[](size_t i) const;

		Vector2& operator+=(const Vector2& a);
		Vector2& operator-=(const Vector2& a);
		Vector2& operator*=(float k);
		Vector2& operator/=(float k);

		union
		{
			struct
			{
				float x;
				float y;
			};
			float data[2];
		};

		static const Vector2 Zero; // {0, 0}
	};

	inline Vector2::Vector2()
	{
		// Do not initialize
	}

	inline Vector2::Vector2(float val) 
		: x(val) 
		, y(val)
	{
	}

	inline Vector2::Vector2(float nx, float ny) 
		: x(nx)
		, y(ny)
	{
	}

	inline Vector2::Vector2(const float a[2]) 
		: x(a[0])
		, y(a[1])
	{
	}

	inline const float& Vector2::operator[](size_t i) const
	{
		RIO_ASSERT(i < 2, "Index out of bounds");
		return data[i];
	}

	inline float& Vector2::operator[](size_t i)
	{
		RIO_ASSERT(i < 2, "Index out of bounds");
		return data[i];
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
		RIO_ASSERT(k != (float)0.0f, "Division by zero");

		float inv = (float)(1.0f / k);

		x *= inv;
		y *= inv;

		return *this;
	}

	Vector2 operator-(const Vector2& a);
	Vector2 operator+(Vector2 a, const Vector2& b);
	Vector2 operator-(Vector2 a, const Vector2& b);
	Vector2 operator*(Vector2 a, float k);
	Vector2 operator*(float k, Vector2 a);

	// Hadamard Product
	Vector2 operator*(const Vector2& a, const Vector2& b);
	// Hadamard Product
	Vector2 operator/(const Vector2& a, const Vector2& b);
	// Divides the vector by the scalar and returns the result.
	Vector2 operator/(Vector2 a, float k);
	// Returns true whether the vectors are equal.
	bool operator==(const Vector2& a, const Vector2& b);
	bool operator!=(const Vector2& a, const Vector2& b);

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

	inline bool operator!=(const Vector2& a, const Vector2& b)
	{
		return !operator==(a, b);
	}

	// Hadamard Product
	inline Vector2 operator*(const Vector2& a, const Vector2& b)
	{
		Vector2 result;
		for (size_t i = 0; i < 2; i++)
			result[i] = a[i] * b[i];
		return result;
	}

	// Hadamard Product
	inline Vector2 operator/(const Vector2& a, const Vector2& b)
	{
		Vector2 result;
		for (size_t i = 0; i < 2; i++)
		{
			result[i] = a[i] / b[i];
		}
		return result;
	}

	namespace Vector2Fn
	{
		float dot(const Vector2& a, const Vector2& b);
		float getLength(const Vector2& a);
		float getLengthSquared(const Vector2& a);
		void setLength(Vector2& a, float len);
		// returns normalized vector
		Vector2 getNormalized(Vector2& a);
		// Returns the distance between the points
		float getDistance(const Vector2& a, const Vector2& b);
		// Returns the angle between the vectors
		Radian getAngle(const Vector2& a, const Vector2& b);
		// Returns the pointer to the data
		float* toFloatPtr(Vector2& a);
		const float* toFloatPtr(const Vector2& a);
		
		inline float dot(const Vector2& a, const Vector2& b)
		{
			return a.x * b.x + a.y * b.y;
		}

		inline float cross(const Vector2& a, const Vector2& b)
		{ 
			return a.x * b.y - b.x * a.y; 
		}
		
		inline float getLengthSquared(const Vector2& a)
		{
			return a.x * a.x + a.y * a.y;
		}
		
		inline void setLength(Vector2& a, float len)
		{
			a = getNormalized(a);

			a.x *= len;
			a.y *= len;
		}

		inline Vector2 getNormalized(Vector2& a)
		{
			float invertedLength = 1.0f / getLength(a);
			return a * invertedLength;
		}

		inline float getDistance(const Vector2& a, const Vector2& b)
		{
			return getLength(b - a);
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
