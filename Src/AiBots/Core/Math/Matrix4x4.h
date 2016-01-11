// Copyright (c) 2015 Volodymyr Syvochka
#pragma once

#include "Core/Base/Config.h"

#include "Core/Base/Types.h"

#include "Core/Math/Vector2.h"
#include "Core/Math/Vector3.h"
#include "Core/Math/Vector4.h"
#include "Core/Math/Quaternion.h"
#include "Core/Math/Matrix3x3.h"

namespace Rio
{
	struct Matrix4x4
	{
		static const Matrix4x4 Identity; // = Matrix4x4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);

		Matrix4x4();
		Matrix4x4(const Vector3& x, const Vector3& y, const Vector3& z, const Vector3& t);
		Matrix4x4(const Vector4& a, const Vector4& b, const Vector4& c, const Vector4& d);
		Matrix4x4(const Quaternion& r, const Vector3& p);
		Matrix4x4(const Matrix3x3& m);

		Matrix4x4(float r1c1, float r2c1, float r3c1, float r4c1,
			float r1c2, float r2c2, float r3c2, float r4c2,
			float r1c3, float r2c3, float r3c3, float r4c3,
			float r1c4, float r2c4, float r3c4, float r4c4);

		Matrix4x4(const float v[16]);

		float& operator[](size_t i);
		const float& operator[](size_t i) const;

		Matrix4x4& operator+=(const Matrix4x4& a);
		Matrix4x4& operator-=(const Matrix4x4& a);
		Matrix4x4& operator*=(const Matrix4x4& a);
		Matrix4x4& operator*=(float k);
		Matrix4x4& operator/=(float k);

		union
		{
			struct
			{
				Vector4 x;
				Vector4 y;
				Vector4 z;
				Vector4 t;
			};
			struct
			{
				Vector4 data[4];
			};
			struct
			{
				float floatData[16];
			};
		};
	};

	inline Matrix4x4::Matrix4x4()
	{
		// Do not initialize
	}

	inline Matrix4x4::Matrix4x4(const Vector3& x, const Vector3& y, const Vector3& z, const Vector3& t)
		: x(Vector4(Vector3(x), 0.0f))
		, y(Vector4(Vector3(y), 0.0f))
		, z(Vector4(Vector3(z), 0.0f))
		, t(Vector4(Vector3(t), 1.0f))
	{
	}

	inline Matrix4x4::Matrix4x4(const Vector4& a, const Vector4& b, const Vector4& c, const Vector4& d)
		: x(a)
		, y(b)
		, z(c)
		, t(d)
	{
	}

	inline Matrix4x4::Matrix4x4(float r1c1, float r2c1, float r3c1, float r4c1,
		float r1c2, float r2c2, float r3c2, float r4c2,
		float r1c3, float r2c3, float r3c3, float r4c3,
		float r1c4, float r2c4, float r3c4, float r4c4)
		: x(Vector4(r1c1, r2c1, r3c1, r4c1))
		, y(Vector4(r1c2, r2c2, r3c2, r4c2))
		, z(Vector4(r1c3, r2c3, r3c3, r4c3))
		, t(Vector4(r1c4, r2c4, r3c4, r4c4))
	{
	}

	inline Matrix4x4::Matrix4x4(const Quaternion& r, const Vector3& p)
		: x(Vector4(1.0f - 2.0f * r.y * r.y - 2.0f * r.z * r.z, 2.0f * r.x * r.y + 2.0f * r.w * r.z, 2.0f * r.x * r.z - 2.0f * r.w * r.y, 0))
		, y(Vector4(2.0f * r.x * r.y - 2.0f * r.w * r.z, 1.0f - 2.0f * r.x * r.x - 2.0f * r.z * r.z, 2.0f * r.y * r.z + 2.0f * r.w * r.x, 0))
		, z(Vector4(2.0f * r.x * r.z + 2.0f * r.w * r.y, 2.0f * r.y * r.z - 2.0f * r.w * r.x, 1.0f - 2.0f * r.x * r.x - 2.0f * r.y * r.y, 0))
		, t(Vector4(p, 1))
	{
	}

	inline Matrix4x4::Matrix4x4(const Matrix3x3& m)
		: x(Vector4(m.x, 0))
		, y(Vector4(m.y, 0))
		, z(Vector4(m.z, 0))
		, t(Vector4(0, 0, 0, 1))
	{
	}

	inline Matrix4x4::Matrix4x4(const float v[16])
		: x(Vector4(v[0], v[1], v[2], v[3]))
		, y(Vector4(v[4], v[5], v[6], v[7]))
		, z(Vector4(v[8], v[9], v[10], v[11]))
		, t(Vector4(v[12], v[13], v[14], v[15]))
	{
	}

	inline float& Matrix4x4::operator[](size_t i)
	{
		RIO_ASSERT(i < 16, "Index out of bounds");
		return floatData[i];
	}

	inline const float& Matrix4x4::operator[](size_t i) const
	{
		RIO_ASSERT(i < 16, "Index out of bounds");
		return floatData[i];
	}

	inline Matrix4x4& Matrix4x4::operator+=(const Matrix4x4& a)
	{
		x += a.x;
		y += a.y;
		z += a.z;
		t += a.t;

		return *this;
	}

	inline Matrix4x4& Matrix4x4::operator-=(const Matrix4x4& a)
	{
		x -= a.x;
		y -= a.y;
		z -= a.z;
		t -= a.t;

		return *this;
	}

	inline Matrix4x4& Matrix4x4::operator*=(float k)
	{
		x *= k;
		y *= k;
		z *= k;
		t *= k;

		return *this;
	}

	inline Matrix4x4& Matrix4x4::operator/=(float k)
	{
		const float invK = 1.0f / k;

		x *= invK;
		y *= invK;
		z *= invK;
		t *= invK;

		return *this;
	}

	inline Matrix4x4& Matrix4x4::operator*=(const Matrix4x4& a)
	{
		Matrix4x4 tmp(
			x.x*a.x.x + x.y*a.y.x + x.z*a.z.x + x.w*a.t.x,
			x.x*a.x.y + x.y*a.y.y + x.z*a.z.y + x.w*a.t.y,
			x.x*a.x.z + x.y*a.y.z + x.z*a.z.z + x.w*a.t.z,
			x.x*a.x.w + x.y*a.y.w + x.z*a.z.w + x.w*a.t.w,

			y.x*a.x.x + y.y*a.y.x + y.z*a.z.x + y.w*a.t.x,
			y.x*a.x.y + y.y*a.y.y + y.z*a.z.y + y.w*a.t.y,
			y.x*a.x.z + y.y*a.y.z + y.z*a.z.z + y.w*a.t.z,
			y.x*a.x.w + y.y*a.y.w + y.z*a.z.w + y.w*a.t.w,

			z.x*a.x.x + z.y*a.y.x + z.z*a.z.x + z.w*a.t.x,
			z.x*a.x.y + z.y*a.y.y + z.z*a.z.y + z.w*a.t.y,
			z.x*a.x.z + z.y*a.y.z + z.z*a.z.z + z.w*a.t.z,
			z.x*a.x.w + z.y*a.y.w + z.z*a.z.w + z.w*a.t.w,

			t.x*a.x.x + t.y*a.y.x + t.z*a.z.x + t.w*a.t.x,
			t.x*a.x.y + t.y*a.y.y + t.z*a.z.y + t.w*a.t.y,
			t.x*a.x.z + t.y*a.y.z + t.z*a.z.z + t.w*a.t.z,
			t.x*a.x.w + t.y*a.y.w + t.z*a.z.w + t.w*a.t.w
			);

		*this = tmp;
		return *this;
	}

	bool operator==(const Matrix4x4& a, const Matrix4x4& b);
	bool operator!=(const Matrix4x4& a, const Matrix4x4& b);
	Matrix4x4 operator+(Matrix4x4 a, const Matrix4x4& b);
	Matrix4x4 operator-(Matrix4x4 a, const Matrix4x4& b);
	Matrix4x4 operator*(Matrix4x4 a, float k);
	Vector4 operator*(const Matrix4x4& a, const Vector4& v);
	Matrix4x4 operator*(float k, Matrix4x4 a);
	Matrix4x4 operator/(Matrix4x4 a, float k);
	Vector3 operator*(const Vector3& v, const Matrix4x4& a);
	Vector4 operator*(const Vector4& v, const Matrix4x4& a);
	// Multiplies the matrix  a by b and returns the result. (i.e. transforms first by a then by b)
	Matrix4x4 operator*(Matrix4x4 a, const Matrix4x4& b);
	Matrix4x4 hadamardProduct(const Matrix4x4& a, const Matrix4x4& b);

	inline bool operator==(const Matrix4x4& a, const Matrix4x4& b)
	{
		for (size_t i = 0; i < 16; i++)
		{
			if (a[i] != b[i])
				return false;
		}
		return true;
	}

	inline bool operator!=(const Matrix4x4& a, const Matrix4x4& b)
	{
		return !operator==(a, b);
	}

	inline Matrix4x4 operator+(Matrix4x4 a, const Matrix4x4& b)
	{
		a += b;
		return a;
	}

	inline Matrix4x4 operator-(Matrix4x4 a, const Matrix4x4& b)
	{
		a -= b;
		return a;
	}

	inline Matrix4x4 operator*(Matrix4x4 a, float k)
	{
		a *= k;
		return a;
	}

	inline Vector4 operator*(const Matrix4x4& a, const Vector4& v)
	{

		const Vector4 mul0 = a[0] * v[0];
		const Vector4 mul1 = a[1] * v[1];
		const Vector4 mul2 = a[2] * v[2];
		const Vector4 mul3 = a[3] * v[3];

		const Vector4 add0 = mul0 + mul1;
		const Vector4 add1 = mul2 + mul3;

		return add0 + add1;
	}
	
	inline Matrix4x4 operator*(float k, Matrix4x4 a)
	{
		a *= k;
		return a;
	}

	inline Matrix4x4 operator/(Matrix4x4 a, float k)
	{
		a /= k;
		return a;
	}

	inline Vector3 operator*(const Vector3& v, const Matrix4x4& a)
	{
		return Vector3(
			v.x*a.x.x + v.y*a.y.x + v.z*a.z.x + a.t.x,
			v.x*a.x.y + v.y*a.y.y + v.z*a.z.y + a.t.y,
			v.x*a.x.z + v.y*a.y.z + v.z*a.z.z + a.t.z
			);
	}

	inline Vector4 operator*(const Vector4& v, const Matrix4x4& a)
	{
		return Vector4(
			v.x*a.x.x + v.y*a.y.x + v.z*a.z.x + v.w*a.t.x,
			v.x*a.x.y + v.y*a.y.y + v.z*a.z.y + v.w*a.t.y,
			v.x*a.x.z + v.y*a.y.z + v.z*a.z.z + v.w*a.t.z,
			v.x*a.x.w + v.y*a.y.w + v.z*a.z.w + v.w*a.t.w
			);
	}

	inline Matrix4x4 operator*(Matrix4x4 a, const Matrix4x4& b)
	{
		a *= b;
		return a;
	}

	inline Matrix4x4 hadamardProduct(const Matrix4x4& a, const Matrix4x4& b)
	{
		Matrix4x4 result;

		for (size_t i = 0; i < 4; i++)
		{
			result[i] = a[i] * b[i];
		}

		return result;
	}

	namespace Matrix4x4Fn
	{
		// Sets the matrix to perspective.
		void setToPerspective(Matrix4x4& m, float fovy, float aspect, float near, float far);
		// Sets the matrix to orthographic.
		void setToOrthographic(Matrix4x4& m, float left, float right, float bottom, float top, float near, float far);
		// Sets the matrix to look.
		void setToLook(Matrix4x4& m, const Vector3& pos, const Vector3& target, const Vector3& up);
		// Transposes the matrix and returns the result.
		Matrix4x4& transpose(Matrix4x4& m);
		Matrix4x4 getTransposed(Matrix4x4 m);
		float getDeterminant(const Matrix4x4& m);
		// Inverts the matrix and returns the result.
		Matrix4x4& invert(Matrix4x4& m);
		// Returns the inverse of the matrix
		Matrix4x4 getInverted(Matrix4x4 m);
		// Sets the matrix to identity.
		void setToIdentity(Matrix4x4& m);
		// Returns the x axis of the matrix
		Vector3 getXAxis(const Matrix4x4& m);
		// Returns the y axis of the matrix
		Vector3 getYAxis(const Matrix4x4& m);
		// Returns the z axis of the matrix
		Vector3 getZAxis(const Matrix4x4& m);
		// Sets the x axis of the matrix
		void setXAxis(Matrix4x4& m, const Vector3& x);
		// Sets the y axis of the matrix
		void setYAxis(Matrix4x4& m, const Vector3& y);
		// Sets the z axis of the matrix
		void setZAxis(Matrix4x4& m, const Vector3& z);
		// Returns the translation portion of the matrix
		Vector3 getTranslation(const Matrix4x4& m);
		// Sets the translation portion of the matrix
		void setTranslation(Matrix4x4& m, const Vector3& trans);
		// Returns the rotation portion of the matrix as a Quaternion.
		Quaternion getRotation(const Matrix4x4& m);
		void setRotation(Matrix4x4& m, const Quaternion& rot);
		void setRotation(Matrix4x4& m, const Matrix3x3& rot);
		Vector3 getScale(const Matrix4x4& m);
		void setScale(Matrix4x4& m, const Vector3& s);
		// Returns the pointer to the matrix's data
		float* toFloatPtr(Matrix4x4& m);
		// Returns the pointer to the first element of the matrix
		const float* toFloatPtr(const Matrix4x4& m);
		// Returns the rotation portion of the matrix as Matrix3x3
		Matrix3x3 toMatrix3x3(const Matrix4x4& m);
		Matrix4x4 quaternionToMatrix4(const Quaternion& q);
		// NOTE: Assumes matrix is only a rotational matrix and has no shear applied
		Quaternion matrix4x4ToQuaternion(const Matrix4x4& m);

		//******************************
		// Transformation
		//******************************

		Matrix4x4 translate(const Vector3& v);
		Matrix4x4 rotate(const Radian& angle, const Vector3& v);
		Matrix4x4 scale(const Vector3& v);

		Matrix4x4 ortho(float left, float right, float bottom, float top);
		Matrix4x4 ortho(float left, float right, float bottom, float top, float zNear, float zFar);

		Matrix4x4 perspective(const Radian& fovy, float aspect, float zNear, float zFar);
		Matrix4x4 infinitePerspective(const Radian& fovy, float aspect, float zNear);

		Matrix4x4 lookAtMatrix4(const Vector3& eye, const Vector3& center, const Vector3& up);
		Quaternion lookAtQuaternion(const Vector3& eye, const Vector3& center, const Vector3& up);

		inline void setToPerspective(Matrix4x4& m, float fovy, float aspect, float near, float far)
		{
			const float height = 1.0f / MathFn::tan(Radian(MathFn::degreesToRadians(fovy)) * 0.5f);
			const float width = height * 1.0f / aspect;
			const float aa = far / (far - near);
			const float bb = -near * aa;

			m.x.x = width;
			m.x.y = 0.0f;
			m.x.z = 0.0f;
			m.x.w = 0.0f;

			m.y.x = 0.0f;
			m.y.y = height;
			m.y.z = 0.0f;
			m.y.w = 0.0f;

			m.z.x = 0.0f;
			m.z.y = 0.0f;
			m.z.z = aa;
			m.z.w = 1.0f;

			m.t.x = 0.0f;
			m.t.y = 0.0f;
			m.t.z = bb;
			m.t.w = 0.0f;
		}

		inline void setToOrthographic(Matrix4x4& m, float left, float right, float bottom, float top, float near, float far)
		{
			m.x.x = 2.0f / (right - left);
			m.x.y = 0.0f;
			m.x.z = 0.0f;
			m.x.w = 0.0f;

			m.y.x = 0.0f;
			m.y.y = 2.0f / (top - bottom);
			m.y.z = 0.0f;
			m.y.w = 0.0f;

			m.z.x = 0.0f;
			m.z.y = 0.0f;
			m.z.z = 1.0f / (far - near);
			m.z.w = 0.0f;

			m.t.x = (left + right) / (left - right);
			m.t.y = (top + bottom) / (bottom - top);
			m.t.z = near / (near - far);
			m.t.w = 1.0f;
		}

		inline Matrix4x4& transpose(Matrix4x4& m)
		{
			float tmp;

			tmp = m.x.y;
			m.x.y = m.y.x;
			m.y.x = tmp;

			tmp = m.x.z;
			m.x.z = m.z.x;
			m.z.x = tmp;

			tmp = m.x.w;
			m.x.w = m.t.x;
			m.t.x = tmp;

			tmp = m.y.z;
			m.y.z = m.z.y;
			m.z.y = tmp;

			tmp = m.y.w;
			m.y.w = m.t.y;
			m.t.y = tmp;

			tmp = m.z.w;
			m.z.w = m.t.z;
			m.t.z = tmp;

			return m;
		}

		inline Matrix4x4 getTransposed(Matrix4x4 m)
		{
			transpose(m);
			return m;
		}

		inline void setToLook(Matrix4x4& m, const Vector3& pos, const Vector3& target, const Vector3& up)
		{
			Vector3 zAxis = pos - target;
			Vector3Fn::normalize(zAxis);
			const Vector3 xAxis = Vector3Fn::cross(up, zAxis);
			const Vector3 yAxis = Vector3Fn::cross(zAxis, xAxis);

			m.x.x = xAxis.x;
			m.x.y = yAxis.x;
			m.x.z = zAxis.x;
			m.x.w = 0;

			m.y.x = xAxis.y;
			m.y.y = yAxis.y;
			m.y.z = zAxis.y;
			m.y.w = 0;

			m.z.x = xAxis.z;
			m.z.y = yAxis.z;
			m.z.z = zAxis.z;
			m.z.w = 0;

			m.t.x = -Vector3Fn::dot(pos, xAxis);
			m.t.y = -Vector3Fn::dot(pos, yAxis);
			m.t.z = -Vector3Fn::dot(pos, zAxis);
			m.t.w = 1;
		}

		inline float getDeterminant(const Matrix4x4& m)
		{
			const float xx = m.x.x;
			const float xy = m.x.y;
			const float xz = m.x.z;
			const float xw = m.x.w;
			const float yx = m.y.x;
			const float yy = m.y.y;
			const float yz = m.y.z;
			const float yw = m.y.w;
			const float zx = m.z.x;
			const float zy = m.z.y;
			const float zz = m.z.z;
			const float zw = m.z.w;
			const float tx = m.t.x;
			const float ty = m.t.y;
			const float tz = m.t.z;
			const float tw = m.t.w;

			float det = 0.0f;

			det += +xx * (yy * (zz*tw - tz*zw) - zy * (yz*tw - tz*yw) + ty * (yz*zw - zz*yw));
			det += -yx * (xy * (zz*tw - tz*zw) - zy * (xz*tw - tz*xw) + ty * (xz*zw - zz*xw));
			det += +zx * (xy * (yz*tw - tz*yw) - yy * (xz*tw - tz*xw) + ty * (xz*yw - yz*xw));
			det += -tx * (xy * (yz*zw - zz*yw) - yy * (xz*zw - zz*xw) + zy * (xz*yw - yz*xw));

			return det;
		}

		inline Matrix4x4& invert(Matrix4x4& m)
		{
			const float xx = m.x.x;
			const float xy = m.x.y;
			const float xz = m.x.z;
			const float xw = m.x.w;
			const float yx = m.y.x;
			const float yy = m.y.y;
			const float yz = m.y.z;
			const float yw = m.y.w;
			const float zx = m.z.x;
			const float zy = m.z.y;
			const float zz = m.z.z;
			const float zw = m.z.w;
			const float tx = m.t.x;
			const float ty = m.t.y;
			const float tz = m.t.z;
			const float tw = m.t.w;

			const float det = getDeterminant(m);
			const float invertedDet = 1.0f / det;

			m.x.x = +(yy * (zz*tw - tz*zw) - zy * (yz*tw - tz*yw) + ty * (yz*zw - zz*yw)) * invertedDet;
			m.x.y = -(xy * (zz*tw - tz*zw) - zy * (xz*tw - tz*xw) + ty * (xz*zw - zz*xw)) * invertedDet;
			m.x.z = +(xy * (yz*tw - tz*yw) - yy * (xz*tw - tz*xw) + ty * (xz*yw - yz*xw)) * invertedDet;
			m.x.w = -(xy * (yz*zw - zz*yw) - yy * (xz*zw - zz*xw) + zy * (xz*yw - yz*xw)) * invertedDet;

			m.y.x = -(yx * (zz*tw - tz*zw) - zx * (yz*tw - tz*yw) + tx * (yz*zw - zz*yw)) * invertedDet;
			m.y.y = +(xx * (zz*tw - tz*zw) - zx * (xz*tw - tz*xw) + tx * (xz*zw - zz*xw)) * invertedDet;
			m.y.z = -(xx * (yz*tw - tz*yw) - yx * (xz*tw - tz*xw) + tx * (xz*yw - yz*xw)) * invertedDet;
			m.y.w = +(xx * (yz*zw - zz*yw) - yx * (xz*zw - zz*xw) + zx * (xz*yw - yz*xw)) * invertedDet;

			m.z.x = +(yx * (zy*tw - ty*zw) - zx * (yy*tw - ty*yw) + tx * (yy*zw - zy*yw)) * invertedDet;
			m.z.y = -(xx * (zy*tw - ty*zw) - zx * (xy*tw - ty*xw) + tx * (xy*zw - zy*xw)) * invertedDet;
			m.z.z = +(xx * (yy*tw - ty*yw) - yx * (xy*tw - ty*xw) + tx * (xy*yw - yy*xw)) * invertedDet;
			m.z.w = -(xx * (yy*zw - zy*yw) - yx * (xy*zw - zy*xw) + zx * (xy*yw - yy*xw)) * invertedDet;

			m.t.x = -(yx * (zy*tz - ty*zz) - zx * (yy*tz - ty*yz) + tx * (yy*zz - zy*yz)) * invertedDet;
			m.t.y = +(xx * (zy*tz - ty*zz) - zx * (xy*tz - ty*xz) + tx * (xy*zz - zy*xz)) * invertedDet;
			m.t.z = -(xx * (yy*tz - ty*yz) - yx * (xy*tz - ty*xz) + tx * (xy*yz - yy*xz)) * invertedDet;
			m.t.w = +(xx * (yy*zz - zy*yz) - yx * (xy*zz - zy*xz) + zx * (xy*yz - yy*xz)) * invertedDet;

			return m;
		}

		inline Matrix4x4 getInverted(Matrix4x4 m)
		{
			invert(m);
			return m;
		}

		inline void setToIdentity(Matrix4x4& m)
		{
			m.x = Vector4(1, 0, 0, 0);
			m.y = Vector4(0, 1, 0, 0);
			m.z = Vector4(0, 0, 1, 0);
			m.t = Vector4(0, 0, 0, 1);
		}

		inline Vector3 getXAxis(const Matrix4x4& m)
		{
			return Vector3(m.x.x, m.x.y, m.x.z);
		}

		inline Vector3 getYAxis(const Matrix4x4& m)
		{
			return Vector3(m.y.x, m.y.y, m.y.z);
		}

		inline Vector3 getZAxis(const Matrix4x4& m)
		{
			return Vector3(m.z.x, m.z.y, m.z.z);
		}

		inline void setXAxis(Matrix4x4& m, const Vector3& x)
		{
			m.x.x = x.x;
			m.x.y = x.y;
			m.x.z = x.z;
		}

		inline void setYAxis(Matrix4x4& m, const Vector3& y)
		{
			m.y.x = y.x;
			m.y.y = y.y;
			m.y.z = y.z;
		}

		inline void setZAxis(Matrix4x4& m, const Vector3& z)
		{
			m.z.x = z.x;
			m.z.y = z.y;
			m.z.z = z.z;
		}

		inline Vector3 getTranslation(const Matrix4x4& m)
		{
			return Vector3(m.t.x, m.t.y, m.t.z);
		}

		inline void setTranslation(Matrix4x4& m, const Vector3& trans)
		{
			m.t.x = trans.x;
			m.t.y = trans.y;
			m.t.z = trans.z;
		}

		inline Quaternion getRotation(const Matrix4x4& m)
		{
			return Matrix3x3Fn::getRotation(toMatrix3x3(m));
		}

		inline void setRotation(Matrix4x4& m, const Quaternion& rot)
		{
			setRotation(m, Matrix3x3(rot));
		}

		inline void setRotation(Matrix4x4& m, const Matrix3x3& rot)
		{
			m.x.x = rot.x.x;
			m.x.y = rot.x.y;
			m.x.z = rot.x.z;
			m.y.x = rot.y.x;
			m.y.y = rot.y.y;
			m.y.z = rot.y.z;
			m.z.x = rot.z.x;
			m.z.y = rot.z.y;
			m.z.z = rot.z.z;
		}

		inline Vector3 getScale(const Matrix4x4& m)
		{
			const float sx = Vector3Fn::getLength(Vector4Fn::toVector3(m.x));
			const float sy = Vector3Fn::getLength(Vector4Fn::toVector3(m.y));
			const float sz = Vector3Fn::getLength(Vector4Fn::toVector3(m.z));
			return Vector3(sx, sy, sz);
		}

		inline void setScale(Matrix4x4& m, const Vector3& s)
		{
			Matrix3x3 rot = toMatrix3x3(m);
			Matrix3x3Fn::setScale(rot, s);
			setRotation(m, rot);
		}

		inline float* toFloatPtr(Matrix4x4& m)
		{
			return Vector4Fn::toFloatPtr(m.x);
		}

		inline const float* toFloatPtr(const Matrix4x4& m)
		{
			return Vector4Fn::toFloatPtr(m.x);
		}

		inline Matrix3x3 toMatrix3x3(const Matrix4x4& m)
		{
			return Matrix3x3(getXAxis(m), getYAxis(m), getZAxis(m));
		}

		inline Matrix4x4 quaternionToMatrix4(const Quaternion& q)
		{
			Matrix4x4 mat = Matrix4x4::Identity;
			const Quaternion a = QuaternionFn::getNormalized(q);

			const float xx = a.x * a.x;
			const float yy = a.y * a.y;
			const float zz = a.z * a.z;
			const float xy = a.x * a.y;
			const float xz = a.x * a.z;
			const float yz = a.y * a.z;
			const float wx = a.w * a.x;
			const float wy = a.w * a.y;
			const float wz = a.w * a.z;

			mat[0 * 4 + 0] = 1.0f - 2.0f * (yy + zz);
			mat[0 * 4 + 1] = 2.0f * (xy + wz);
			mat[0 * 4 + 2] = 2.0f * (xz - wy);

			mat[1 * 4 + 0] = 2.0f * (xy - wz);
			mat[1 * 4 + 1] = 1.0f - 2.0f * (xx + zz);
			mat[1 * 4 + 2] = 2.0f * (yz + wx);

			mat[2 * 4 + 0] = 2.0f * (xz + wy);
			mat[2 * 4 + 1] = 2.0f * (yz - wx);
			mat[2 * 4 + 2] = 1.0f - 2.0f * (xx + yy);

			return mat;
		}
	} // namespace Matrix4x4Fn
} // namespace Rio
