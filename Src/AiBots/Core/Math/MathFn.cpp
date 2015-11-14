#include "Core/Math/MathFn.h"
#include "Core/Base/Common.h" // pseudo_cast

#include <cmath>
#include <cfenv>

namespace Rio
{
	namespace MathFn
	{
		// Trigonometric
		float sin(const Radian& theta) 
		{ 
			return std::sin(static_cast<float>(theta)); 
		}

		float cos(const Radian& theta) 
		{
			return std::cos(static_cast<float>(theta)); 
		}

		float tan(const Radian& theta) 
		{ 
			return std::tan(static_cast<float>(theta)); 
		}

		Radian asin(float a) 
		{
			return Radian(std::asin(a)); 
		}

		Radian acos(float a) 
		{
			return Radian(std::acos(a)); 
		}

		Radian atan(float a) 
		{
			return Radian(std::atan(a)); 
		}

		Radian atan2(float y, float x) 
		{
			return Radian(std::atan2(y, x)); 
		}

		// Hyperbolic
		float sinh(float x) { return std::sinh(x); }
		float cosh(float x) { return std::cosh(x); }
		float tanh(float x) { return std::tanh(x); }

		float asinh(float x) { return std::asinh(x); }
		float acosh(float x) { return std::acosh(x); }
		float atanh(float x) { return std::atanh(x); }

		// Power
		float pow(float x, float y) { return std::pow(x, y); }

		float sqrt(float x) { return std::sqrt(x); }

		float cbrt(float x) { return std::cbrt(x); }

		float hypotenuse(float x, float y) { return std::hypot(x, y); }

		float fastInvSqrt(float number)
		{
			const float threeHalfs = 1.5f;

			float x2 = number * 0.5f;
			float y = number;
			uint32_t i = pseudo_cast<uint32_t>(y); // Evil floating point bit level hacking
			//	i = 0x5f3759df - (i >> 1);           // What the fuck?
			i = 0x5f375a86 - (i >> 1); // What the fuck? Improved!
			y = pseudo_cast<float>(i);
			y = y * (threeHalfs - (x2 * y * y)); // 1st iteration
			//	y = y * (threeHalfs - (x2 * y * y)); // 2nd iteration, this can be
			// removed

			return y;
		}

		// Exponential and Logarithm
		float exp(float x) // e^x
		{
			return std::exp(x);
		}
		float exp2(float x) // 2^x
		{
			return std::exp2(x);
		}
		float ln(float x) { return std::log(x); }
		float ln1p(float x) // ln(1 + x)
		{
			return std::log1p(x);
		}
		float log2(float x) { return std::log2(x); }

		float log10(float x) { return std::log10(x); }
		float logBase(float x, float base) { return ln(x) * (1.0f / ln(base)); }

		// Rounding
		float ceil(float x) { return std::ceil(x); }
		float floor(float x) { return std::floor(x); }
		float mod(float x, float y)
		{
			float result = std::remainder(MathFn::abs(x), (y = MathFn::abs(y)));
			if (std::signbit(result))
				result += y;
			return std::copysign(result, x);
		}
		float truncate(float x)
		{
			uint32_t i = pseudo_cast<uint32_t>(x);
			uint32_t exponent = (i >> 23) & 0xFF; // extract exponent field
			uint32_t fractionalBits = 0x7F + 23 - exponent;
			if (fractionalBits > 23) // abs(x) < 1.0f
				return 0.0f;
			if (fractionalBits > 0)
				i &= ~((1u << fractionalBits) - 1);
			return pseudo_cast<float>(i);
		}
		float round(float x)
		{
			std::fenv_t saveEnv;
			std::feholdexcept(&saveEnv);
			float result = std::rint(x);
			if (std::fetestexcept(FE_INEXACT))
			{
				std::fesetround(FE_TOWARDZERO);
				result = std::rint(std::copysign(0.5f + MathFn::abs(x), x));
			}
			std::feupdateenv(&saveEnv);
			return result;
		}

		int32_t sign(int32_t x)
		{
			uint32_t i = reinterpret_cast<const uint32_t&>(x);
			i &= 0x80000000ul;
			if (i)
				return -1;
			return 1;
		}

		int64_t sign(int64_t x)
		{
			uint64_t i = reinterpret_cast<const uint64_t&>(x);
			i &= 0x8000000000000000ull;
			if (i)
				return -1;
			return 1;
		}

		float sign(float x)
		{
			uint32_t i = reinterpret_cast<const uint32_t&>(x);
			i &= 0x80000000ul;
			if (i)
				return -1.0f;
			return 1.0f;
		}

		// Other
		float abs(float x)
		{
			uint32_t i = reinterpret_cast<const uint32_t&>(x);
			i &= 0x7FFFFFFFul;
			return reinterpret_cast<const float&>(i);
		}

		int8_t abs(int8_t x)
		{
			uint8_t i = reinterpret_cast<const uint8_t&>(x);
			i &= 0x7Fu;
			return reinterpret_cast<const int8_t&>(i);
		}

		int16_t abs(int16_t x)
		{
			uint16_t i = reinterpret_cast<const uint16_t&>(x);
			i &= 0x7FFFu;
			return reinterpret_cast<const int16_t&>(i);
		}

		int32_t abs(int32_t x)
		{
			uint32_t i = reinterpret_cast<const uint32_t&>(x);
			i &= 0x7FFFFFFFul;
			return reinterpret_cast<const int32_t&>(i);
		}

		int64_t abs(int64_t x)
		{
			uint64_t i = reinterpret_cast<const uint64_t&>(x);
			i &= 0x7FFFFFFFFFFFFFFFull;
			return reinterpret_cast<const int64_t&>(i);
		}

		Radian abs(const Radian& x) 
		{ 
			return Radian(abs(static_cast<float>(x))); 
		}

		Degree abs(const Degree& x) 
		{ 
			return Degree(abs(static_cast<float>(x))); 
		}

	} // namespace MathFn
} // namespace Rio
