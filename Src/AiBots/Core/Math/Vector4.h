#pragma once

#include "Core/Base/Config.h"

#include "Core/Debug/Error.h"
#include "Core/Base/Types.h"
#include "Core/Math/MathUtils.h"
#include "Core/Math/Vector3.h"

namespace Rio
{

	struct Vector4
	{
		Vector4();
		Vector4(const Vector3& a, float w);
		Vector4(float val);
		Vector4(float nx, float ny, float nz, float nw);
		Vector4(const float v[4]);

		float& operator[](uint32_t i);
		const float& operator[](uint32_t i) const;

		Vector4& operator+=(const Vector4& a);
		Vector4& operator-=(const Vector4& a);
		Vector4& operator*=(float k);
		Vector4& operator/=(float k);

		float x, y, z, w;
	};

	inline Vector4::Vector4()
	{
		// Do not initialize
	}

	inline Vector4::Vector4(const Vector3& a, float w)
		: x(a.x), y(a.y), z(a.z), w(w)
	{
	}

	inline Vector4::Vector4(float val) : x(val), y(val), z(val), w(val)
	{
	}

	inline Vector4::Vector4(float nx, float ny, float nz, float nw) : x(nx), y(ny), z(nz), w(nw)
	{
	}

	inline Vector4::Vector4(const float a[4]) : x(a[0]), y(a[1]), z(a[2]), w(a[3])
	{
	}

	inline const float& Vector4::operator[](uint32_t i) const
	{
		RIO_ASSERT(i < 4, "Index out of bounds");

		return (&x)[i];
	}

	inline float& Vector4::operator[](uint32_t i)
	{
		RIO_ASSERT(i < 4, "Index out of bounds");

		return (&x)[i];
	}

	inline Vector4& Vector4::operator+=(const Vector4& a)
	{
		x += a.x;
		y += a.y;
		z += a.z;
		w += a.w;

		return *this;
	}

	inline Vector4& Vector4::operator-=(const Vector4& a)
	{
		x -= a.x;
		y -= a.y;
		z -= a.z;
		w -= a.w;

		return *this;
	}

	inline Vector4& Vector4::operator*=(float k)
	{
		x *= k;
		y *= k;
		z *= k;
		w *= k;

		return *this;
	}

	inline Vector4& Vector4::operator/=(float k)
	{
		RIO_ASSERT(k != (float)0.0, "Division by zero");

		float inv = (float)(1.0 / k);

		x *= inv;
		y *= inv;
		z *= inv;
		w *= inv;

		return *this;
	}

	Vector4 operator-(const Vector4& a);
	Vector4 operator+(Vector4 a, const Vector4& b);
	Vector4 operator-(Vector4 a, const Vector4& b);
	Vector4 operator*(Vector4 a, float k);
	Vector4 operator*(float k, Vector4 a);
	Vector4 operator/(Vector4 a, float k);
	bool operator==(const Vector4& a, const Vector4& b);

	inline Vector4 operator-(const Vector4& a)
	{
		return Vector4(-a.x, -a.y, -a.z, -a.w);
	}

	inline Vector4 operator+(Vector4 a, const Vector4& b)
	{
		a += b;
		return a;
	}

	inline Vector4 operator-(Vector4 a, const Vector4& b)
	{
		a -= b;
		return a;
	}

	inline Vector4 operator*(Vector4 a, float k)
	{
		a *= k;
		return a;
	}

	inline Vector4 operator*(float k, Vector4 a)
	{
		a *= k;
		return a;
	}

	inline Vector4 operator/(Vector4 a, float k)
	{
		a /= k;
		return a;
	}

	inline bool operator==(const Vector4& a, const Vector4& b)
	{
		return equals(a.x, b.x) && equals(a.y, b.y) && equals(a.z, b.z) && equals(a.w, b.w);
	}

	namespace Vector4Fn
	{
		const Vector4 ZERO = Vector4(0, 0, 0, 0);
		const Vector4 XAXIS = Vector4(1, 0, 0, 0);
		const Vector4 YAXIS = Vector4(0, 1, 0, 0);
		const Vector4 ZAXIS = Vector4(0, 0, 1, 0);
		const Vector4 WAXIS = Vector4(0, 0, 0, 1);

		/// Returns the dot product
		float dot(const Vector4& a, const Vector4& b);
		float getLength(const Vector4& a);

		float getLengthSquared(const Vector4& a);
		void setLength(Vector4& a, float len);
		Vector4 normalize(Vector4& a);
		float getDistance(const Vector4& a, const Vector4& b);
		float getAngle(const Vector4& a, const Vector4& b);
		float* toFloatPtr(Vector4& a);
		const float* toFloatPtr(const Vector4& a);
		// Returns the Vector3 portion (truncates w)
		Vector3 toVector3(const Vector4& a);

		inline float dot(const Vector4& a, const Vector4& b)
		{
			return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
		}

		inline float getLength(const Vector4& a)
		{
			return sqrt(a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w);
		}

		inline float getLengthSquared(const Vector4& a)
		{
			return a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w;
		}

		inline void setLength(Vector4& a, float len)
		{
			normalize(a);

			a.x *= len;
			a.y *= len;
			a.z *= len;
			a.w *= len;
		}

		inline Vector4 normalize(Vector4& a)
		{
			float inv_len = 1.0f / getLength(a);

			a.x *= inv_len;
			a.y *= inv_len;
			a.z *= inv_len;
			a.w *= inv_len;

			return a;
		}

		inline float getDistance(const Vector4& a, const Vector4& b)
		{
			return getLength(b - a);
		}

		inline float getAngle(const Vector4& a, const Vector4& b)
		{
			return acos(dot(a, b) / (getLength(a) * getLength(b)));
		}

		inline float* toFloatPtr(Vector4& a)
		{
			return &a.x;
		}

		inline const float* toFloatPtr(const Vector4& a)
		{
			return &a.x;
		}

		inline Vector3 toVector3(const Vector4& a)
		{
			return Vector3(a.x, a.y, a.z);
		}
	}

} // namespace Rio
