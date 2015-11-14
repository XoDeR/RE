// Copyright (c) 2015 Volodymyr Syvochka
#pragma once

#include "Core/Base/Config.h"

#include "Core/Debug/Error.h"
#include "Core/Base/Types.h"

#include <math.h>

namespace Rio
{
	namespace MathFn
	{
		extern const float Epsilon;

		extern const float Zero;
		extern const float One;
		extern const float Third;
		extern const float TwoThirds;

		extern const float E;
		extern const float Pi;
		extern const float Tau;

		extern const float Sqrt2;
		extern const float Sqrt3;

		extern const float GoldenRatio;

		const float TWO_PI = Pi * 2.0f;
		const float HALF_PI = Pi * 0.5f;
		const float FLOAT_PRECISION = 1.0e-7f;
		const double DOUBLE_PRECISION = 1.0e-9;
	} // namespace Math

} // namespace Rio