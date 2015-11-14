// Copyright (c) 2015 Volodymyr Syvochka
#include "Core/Math/Plane.h"
#include "Core/Math/MathFn.h"

namespace Rio
{
	const Plane Plane::Zero = Plane(Vector3::Zero, 0.0);
	const Plane Plane::Xaxis = Plane(Vector3::Xaxis, 0.0);
	const Plane Plane::Yaxis = Plane(Vector3::Yaxis, 0.0);
	const Plane Plane::Zaxis = Plane(Vector3::Zaxis, 0.0);


	Plane& PlaneFn::normalize(Plane& p)
	{
		float len = Vector3Fn::getLength(p.n);

		if (MathFn::equals(len, (float) 0.0))
		{
			return p;
		}

		const float invLen = (float) 1.0f / len;

		p.n *= invLen;
		p.d *= invLen;

		return p;
	}

} // namespace Rio
