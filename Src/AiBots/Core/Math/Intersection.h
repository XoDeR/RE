// Copyright (c) 2015 Volodymyr Syvochka
#pragma once

#include "Core/Base/Config.h"

#include "Core/Base/Types.h"

namespace Rio
{
	class Vector3;
	class Plane;
	class Sphere;
	class Aabb;
	class Frustum;

	// Returns the distance along ray to intersection point with disc defined by
	// or -1.0 if no intersection.
	float getRayWithDiscIntersection(const Vector3& from, const Vector3& dir, const Vector3& center, float radius, const Vector3& normal);
	// Returns the distance along ray (from, dir) to intersection point with plane or -1.0 if no intersection.
	float getRayWithPlaneIntersection(const Vector3& from, const Vector3& dir, const Plane& p);
	// Returns the distance along ray (from, dir) to intersection point with sphere or -1.0 if no intersection.
	float getRayWithSphereIntersection(const Vector3& from, const Vector3& dir, const Sphere& s);
	// Returns the distance along ray (from, dir) to intersection point with obb or -1.0 if no intersection.
	float getRayWithOobIntersection(const Vector3& from, const Vector3& dir, const Matrix4x4& tm, const Vector3& halfExtents);
	bool getPlanesIntersection(const Plane& p1, const Plane& p2, const Plane& p3, Vector3& ip);
	bool getFrustumWithSphereIntersection(const Frustum& f, const Sphere& s);
	bool getFrustumWithAaabIntersection(const Frustum& f, const Aabb& b);

} // namespace Rio
