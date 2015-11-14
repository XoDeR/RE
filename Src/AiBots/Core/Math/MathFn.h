// Copyright (c) 2015 Volodymyr Syvochka
#pragma once

#include "Core/Base/Config.h"

#include "Core/Debug/Error.h"
#include "Core/Base/Types.h"
#include "Core/Math/MathConstants.h"


#include "Core/Math/Angle.h"

#include <math.h>

namespace Rio
{
	namespace MathFn
	{
		inline bool equals(float a, float b, float precision = FLOAT_PRECISION)
		{
			return ((b <= (a + precision)) && (b >= (a - precision)));
		}

		inline bool equals(double a, double b, double precision = DOUBLE_PRECISION)
		{
			return ((b <= (a + precision)) && (b >= (a - precision)));
		}

		template <typename T>
		inline void swap(T& a, T& b)
		{
			T tmp = a;
			a = b;
			b = tmp;
		}

		inline float degreesToRadians(float deg)
		{
			return deg * float(MathFn::Pi / 180.0);
		}

		inline float radiansToDegrees(float rad)
		{
			return rad * float(180.0 / MathFn::Pi);
		}

		inline uint32_t getNextPow2(uint32_t x)
		{
			x--;

			x = (x >> 1) | x;
			x = (x >> 2) | x;
			x = (x >> 4) | x;
			x = (x >> 8) | x;
			x = (x >> 16) | x;

			return ++x;
		}

		// is power of 2
		inline bool isPow2(uint32_t x)
		{
			return !(x & (x - 1)) && x;
		}

		inline float sin(float x)
		{
			return sinf(x);
		}

		inline float cos(float x)
		{
			return cosf(x);
		}

		inline float fmod(float n, float d)
		{
			return ::fmod(n, d);
		}

		// Returns the linear interpolated value between p0 and p1 at time t
		template <typename T>
		inline T getLinearInterpolation(const T& p0, const T& p1, float t)
		{
			return p0 + (t * (p1 - p0));
		}

		// Returns the cosine interpolated value between p0 and p1 at time t
		template <typename T>
		inline T getCosineInterpolation(const T& p0, const T& p1, float t)
		{
			float f = t * MathFn::Pi;
			float g = (1.0 - cos(Radian(f))) * 0.5;

			return p0 + (g * (p1 - p0));
		}

		template <typename T>
		inline T getCubicInterpolation(const T& p0, const T& p1, float t)
		{
			float tt = t * t;
			float ttt = tt * t;

			return p0 * (2.0 * ttt - 3.0 * tt + 1.0) + p1 * (3.0 * tt - 2.0 * ttt);
		}

		template <typename T>
		inline T getBezierInterpolation(const T& p0, const T& p1, const T& p2, const T& p3, float t)
		{
			float u = 1.0 - t;
			float tt = t * t;
			float uu = u * u;
			float uuu = uu * u;
			float ttt = tt * t;

			T tmp = (uuu * p0) +
				(3 * uu * t * p1) +
				(3 * u * tt * p2) +
				(ttt * p3);

			return tmp;
		}

		template <typename T>
		inline T getCatmullRomInterpolation(const T& p0, const T& p1, const T& p2, const T& p3, float t)
		{
			float tt = t * t;
			float ttt = tt * t;

			T tmp = (2.0 * p1) +
				((-p0 + p2) * t) +
				(((2.0 * p0) - (5.0 * p1) + (4.0 * p2) - p3) * tt) +
				((-p0 + (3.0 * p1) + (-3.0 * p2) + p3) * ttt);

			return tmp * 0.5;
		}


		//**********************
		// Common
		//**********************

		// Trigonometric
		float sin(const Radian& theta);
		float cos(const Radian& theta);
		float tan(const Radian& theta);

		Radian asin(float a);
		Radian acos(float a);
		Radian atan(float a);
		Radian atan2(float y, float x);

		// Hyperbolic
		float sinh(float x);
		float cosh(float x);
		float tanh(float x);

		float asinh(float x);
		float acosh(float x);
		float atanh(float x);

		// Power
		float pow(float x, float y);
		float sqrt(float x);

		inline float inverseSqrt(float x)
		{
			return 1.0f / MathFn::sqrt(x);
		}

		float cbrt(float x);
		float hypotenuse(float x, float y);

		float fastInvSqrt(float x);

		// Exponential and Logarithm
		float exp(float x);  // e^x
		float exp2(float x); // 2^x
		float ln(float x);
		float ln1p(float x); // ln(1 + x)
		float log2(float x);
		float log10(float x);
		float logBase(float x, float base);

		// Rounding
		float ceil(float x);
		float floor(float x);
		float mod(float x, float y);
		float truncate(float x);
		float round(float x);

		int32_t sign(int32_t x);
		int64_t sign(int64_t x);
		float sign(float x);

		// Other
		float abs(float x);
		int8_t abs(int8_t x);
		int16_t abs(int16_t x);
		int32_t abs(int32_t x);
		int64_t abs(int64_t x);

		Radian abs(const Radian& x);
		Degree abs(const Degree& x);


		template <typename T>
		inline const T& min(const T& a, const T& b)
		{
			return (b < a) ? b : a;
		}

		template <typename T>
		inline const T& max(const T& a, const T& b)
		{
			return (a < b) ? b : a;
		}

		template <typename T>
		T clamp(const T& x, const T& lower, const T& upper)
		{
			RIO_ASSERT(lower < upper, "Min must be < max");
			return MathFn::max(lower, MathFn::min(x, upper));
		}

		template <typename T>
		inline T kroneckerDelta(const T& i, const T& j)
		{
			return i == j ? T(1) : T(0);
		}

		//*****************
		// Interpolation
		//*****************

		// Linear Interpolation
		template <typename T>
		inline T lerp(const T& x, const T& y, float t)
		{
			RIO_ASSERT(t >= 0.0f && t <= 1.0f, "MathFn::lerp `t` out range (0..1).");

			return x * (1.0f - t) + (y * t);
		}




	} // namespace MathFn
} // namespace Rio
