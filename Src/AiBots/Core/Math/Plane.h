// Copyright (c) 2015 Volodymyr Syvochka
#pragma once

#include "Core/Base/Config.h"

#include "Core/Base/Types.h"

#include "Core/Math/Vector3.h"

namespace Rio
{

	// 3D Plane.
	// The form is ax + by + cz + d = 0
	// where: d = -Vector3Fn::dot(n, p)
	struct Plane
	{
		// Not optimized
		Plane();
		Plane(const Vector3& n, float d);
		Vector3 n;
		float d;
	};

	inline Plane::Plane()
	{
		// Do not initialize
	}

	inline Plane::Plane(const Vector3& normal, float dist)
		: n(normal), d(dist)
	{
	}

	namespace PlaneFn
	{
		// Normalizes the plane and returns its result.
		Plane& normalize(Plane& p);
		// Returns the signed distance between plane and point
		float getDistanceToPoint(const Plane& p, const Vector3& point);

		const Plane ZERO = Plane(Vector3Fn::ZERO, 0.0);
		const Plane	XAXIS = Plane(Vector3Fn::XAXIS, 0.0);
		const Plane	YAXIS = Plane(Vector3Fn::YAXIS, 0.0);
		const Plane	ZAXIS = Plane(Vector3Fn::ZAXIS, 0.0);

		inline Plane& normalize(Plane& p)
		{
			float len = Vector3Fn::getLength(p.n);

			if (equals(len, (float) 0.0))
			{
				return p;
			}

			const float inv_len = (float) 1.0 / len;

			p.n *= inv_len;
			p.d *= inv_len;

			return p;
		}

		inline float getDistanceToPoint(const Plane& p, const Vector3& point)
		{
			return Vector3Fn::dot(p.n, point) + p.d;
		}
	} // namespace PlaneFn

} // namespace Rio
