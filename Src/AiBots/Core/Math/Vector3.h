#pragma once

#include "Core/Base/Config.h"

#include "Core/Debug/Error.h"
#include "Core/Base/Types.h"
#include "Core/Math/Vector2.h"
#include "Core/Math/Angle.h"

namespace Rio
{
	struct Vector3
	{
		Vector3();
		Vector3(float val);
		Vector3(float nx, float ny, float nz);
		Vector3(const float v[3]);

		float& operator[](size_t i);
		const float& operator[](size_t i) const;

		Vector3& operator+=(const Vector3& a);
		Vector3& operator-=(const Vector3& a);
		Vector3& operator*=(float k);
		Vector3& operator/=(float k);

		union
		{
			struct
			{
				float x;
				float y;
				float z;
			};
			struct
			{
				float r;
				float g;
				float b;
			};
			float data[3];
			struct
			{
				Vector2 xy;
			};
		};

		static const Vector3 Zero;
		static const Vector3 Xaxis;// = Vector3(1, 0, 0);
		static const Vector3 Yaxis;// = Vector3(0, 1, 0);
		static const Vector3 Zaxis;// = Vector3(0, 0, 1);
		static const Vector3 FORWARD;// = Vector3(0, 0, 1);
		static const Vector3 BACKWARD;// = Vector3(0, 0, -1);
		static const Vector3 LEFT;// = Vector3(-1, 0, 0);
		static const Vector3 RIGHT;// = Vector3(1, 0, 0);
		static const Vector3 UP;// = Vector3(0, 1, 0);
		static const Vector3 DOWN;// = Vector3(0, -1, 0);
	};

	inline Vector3::Vector3()
	{
		// Do not initialize
	}

	inline Vector3::Vector3(float val) 
		: x(val)
		, y(val)
		, z(val)
	{
	}

	inline Vector3::Vector3(float nx, float ny, float nz) 
		: x(nx)
		, y(ny)
		, z(nz)
	{
	}

	inline Vector3::Vector3(const float v[3]) 
		: x(v[0])
		, y(v[1])
		, z(v[2])
	{
	}

	inline const float& Vector3::operator[](size_t i) const
	{
		RIO_ASSERT(i < 3, "Index out of bounds");
		return data[i];
	}

	inline float& Vector3::operator[](size_t i)
	{
		RIO_ASSERT(i < 3, "Index out of bounds");
		return data[i];
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
		RIO_ASSERT(k != (float)0.0f, "Division by zero");

		float inv = (float)(1.0f / k);

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

	bool operator==(const Vector3& a, const Vector3& b);
	bool operator!=(const Vector3& a, const Vector3& b);

	// Hadamard Product
	Vector3 operator*(const Vector3& a, const Vector3& b);
	// Hadamard Product
	Vector3 operator/(const Vector3& a, const Vector3& b);

	Vector3 getNormalized(const Vector3& a);

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

	inline bool operator!=(const Vector3& a, const Vector3& b)
	{
		return !operator==(a, b);
	}

	// Hadamard Product
	inline Vector3 operator*(const Vector3& a, const Vector3& b)
	{
		Vector3 result;
		for (size_t i = 0; i < 3; i++)
		{
			result[i] = a[i] * b[i];
		}
		return result;
	}

	// Hadamard Product
	inline Vector3 operator/(const Vector3& a, const Vector3& b)
	{
		Vector3 result;
		for (size_t i = 0; i < 3; i++)
		{
			result[i] = a[i] / b[i];
		}
		return result;
	}

	namespace Vector3Fn
	{
		float dot(const Vector3& a, const Vector3& b);
		Vector3 cross(const Vector3& a, const Vector3& b);
		float getLength(const Vector3& a);
		float getLengthSquared(const Vector3& a);
		void setLength(Vector3& a, float len);
		Vector3 normalize(Vector3& a);
		float getDistance(const Vector3& a, const Vector3& b);
		
		inline float getDistanceSquared(const Vector3& a, const Vector3& b)
		{
			return getLengthSquared(b - a);
		}

		Radian getAngle(const Vector3& a, const Vector3& b);
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
			float invLen = 1.0f / getLength(a);

			a.x *= invLen;
			a.y *= invLen;
			a.z *= invLen;

			return a;
		}

		inline float getDistance(const Vector3& a, const Vector3& b)
		{
			return getLength(b - a);
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

		// Returns a vector that contains the largest value for each component
		inline Vector3 getMax(const Vector3& a, const Vector3& b)
		{
			Vector3 v;
			v.x = MathFn::max(a.x, b.x);
			v.y = MathFn::max(a.y, b.y);
			v.z = MathFn::max(a.z, b.z);
			return v;
		}

		// Returns a vector that contains the smallest value for each component
		inline Vector3 getMin(const Vector3& a, const Vector3& b)
		{
			Vector3 v;
			v.x = MathFn::min(a.x, b.x);
			v.y = MathFn::min(a.y, b.y);
			v.z = MathFn::min(a.z, b.z);
			return v;
		}

		// Returns the linearly interpolated vector at time t in [0, 1]
		inline Vector3 getLerp(const Vector3& a, const Vector3& b, float t)
		{
			Vector3 v;
			v.x = MathFn::lerp(a.x, b.x, t);
			v.y = MathFn::lerp(a.y, b.y, t);
			v.z = MathFn::lerp(a.z, b.z, t);
			return v;
		}
	} // namespace Vector3Fn

} // namespace Rio
