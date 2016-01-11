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
		static const Plane Zero; //= Plane(Vector3::Zero, 0.0);
		static const Plane Xaxis; //= Plane(Vector3Fn::Xaxis, 0.0);
		static const Plane Yaxis; //= Plane(Vector3Fn::Yaxis, 0.0);
		static const Plane Zaxis; //= Plane(Vector3Fn::Zaxis, 0.0);

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
		Plane& normalize(Plane& p);
		// Returns the signed distance between plane and point
		float getDistanceToPoint(const Plane& p, const Vector3& point);

		inline Plane createPlaneFromPointAndNormal(const Vector3& point, const Vector3& normal)
		{
			Plane p;
			p.n = normal;
			p.d = -Vector3Fn::dot(normal, point);
			return p;
		}

		inline float getDistanceToPoint(const Plane& p, const Vector3& point)
		{
			return Vector3Fn::dot(p.n, point) + p.d;
		}
	} // namespace PlaneFn

} // namespace Rio
