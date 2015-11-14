// Copyright (c) 2015 Volodymyr Syvochka
#pragma once

#include "Core/Base/Config.h"

#include "Core/Base/Types.h"

namespace Rio
{
	class Vector3;
	class Plane;
	class Sphere;
	class Obb;
	class Aabb;
	class Frustum;

	// Returns the distance along ray (from, dir) to intersection point with plane
	// or -1.0 if no intersection.
	float getRayWithPlaneIntersection(const Vector3& from, const Vector3& dir, const Plane& p);
	// Returns the distance along ray (from, dir) to intersection point with sphere
	// or -1.0 if no intersection.
	float getRayWithSphereIntersection(const Vector3& from, const Vector3& dir, const Sphere& s);
	// Returns the distance along ray (from, dir) to intersection point with obb
	// or -1.0 if no intersection.
	float getRayWithOobIntersection(const Vector3& from, const Vector3& dir, const Obb& obb);
	bool getPlanesIntersection(const Plane& p1, const Plane& p2, const Plane& p3, Vector3& ip);
	bool getFrustumWithSphereIntersection(const Frustum& f, const Sphere& s);
	bool getFrustumWithAaabIntersection(const Frustum& f, const Aabb& b);

} // namespace Rio
