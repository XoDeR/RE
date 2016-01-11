// Copyright (c) 2015 Volodymyr Syvochka
#include "Core/Math/Intersection.h"

#include "Core/Math/Vector3.h"
#include "Core/Math/Obb.h"
#include "Core/Math/Aabb.h"
#include "Core/Math/Plane.h"
#include "Core/Math/Frustum.h"
#include "Core/Math/Sphere.h"

#include "Core/Math/MathFn.h"

namespace Rio
{
	float getRayWithPlaneIntersection(const Vector3& from, const Vector3& dir, const Plane& p)
	{
		const float num = Vector3Fn::dot(from, p.n);
		const float den = Vector3Fn::dot(dir, p.n);

		if (MathFn::equals(den, 0.0f))
		{
			return -1.0f;
		}

		return (-p.d - num) / den;
	}

	float getRayWithDiscIntersection(const Vector3& from, const Vector3& dir, const Vector3& center, float radius, const Vector3& normal)
	{
		const Plane p = PlaneFn::createPlaneFromPointAndNormal(center, normal);
		const float t = getRayWithPlaneIntersection(from, dir, p);

		if (t == -1.0f)
		{
			return -1.0f;
		}

		const Vector3 intersectionPoint = from + dir * t;
		if (Vector3Fn::getDistanceSquared(intersectionPoint, center) < radius*radius)
		{
			return t;
		}

		return -1.0f;
	}

	float getRayWithSphereIntersection(const Vector3& from, const Vector3& dir, const Sphere& s)
	{
		const Vector3 v = s.c - from;
		const float b = Vector3Fn::dot(v, dir);
		const float det = (s.r * s.r) - Vector3Fn::dot(v, v) + (b * b);
		if (det < 0.0 || b < s.r)
		{
			return -1.0f;
		}
		return b - MathFn::sqrt(det);
	}

	float getRayWithOobIntersection(const Vector3& from, const Vector3& dir, const Matrix4x4& tm, const Vector3& halfExtents)
	{
		using namespace Vector3Fn;

		float tmin = 0.0f;
		float tmax = 100000.0f;

		const Vector3 obb_pos = Vector3(tm.t.x, tm.t.y, tm.t.z);
		const Vector3 delta = obb_pos - from;
		{
			const Vector3 xaxis = Vector3(tm.x.x, tm.x.y, tm.x.z);
			const float e = dot(xaxis, delta);
			const float f = dot(dir, xaxis);

			if (fabs(f) > 0.001f)
			{
				float t1 = (e - halfExtents.x) / f;
				float t2 = (e + halfExtents.x) / f;

				if (t1 > t2)
				{
					float w = t1; t1 = t2; t2 = w;
				}

				if (t2 < tmax)
				{
					tmax = t2;
				}
				if (t1 > tmin)
				{
					tmin = t1;
				}

				if (tmax < tmin)
				{
					return -1.0f;
				}

			}
			else
			{
				if (-e - halfExtents.x > 0.0f || -e + halfExtents.x < 0.0f)
				{
					return -1.0f;
				}
			}
		}

		{
			const Vector3 yaxis = Vector3(tm.y.x, tm.y.y, tm.y.z);
			const float e = dot(yaxis, delta);
			const float f = dot(dir, yaxis);

			if (fabs(f) > 0.001f)
			{

				float t1 = (e - halfExtents.y) / f;
				float t2 = (e + halfExtents.y) / f;

				if (t1 > t2)
				{
					float w = t1; t1 = t2; t2 = w;
				}

				if (t2 < tmax)
					tmax = t2;
				if (t1 > tmin)
					tmin = t1;

				if (tmin > tmax)
					return -1.0f;
			}
			else
			{
				if (-e - halfExtents.y > 0.0f || -e + halfExtents.y < 0.0f)
					return -1.0f;
			}
		}

		{
			const Vector3 zaxis = Vector3(tm.z.x, tm.z.y, tm.z.z);
			const float e = dot(zaxis, delta);
			const float f = dot(dir, zaxis);

			if (fabs(f) > 0.001f)
			{

				float t1 = (e - halfExtents.z) / f;
				float t2 = (e + halfExtents.z) / f;

				if (t1 > t2){ float w = t1; t1 = t2; t2 = w; }

				if (t2 < tmax)
					tmax = t2;
				if (t1 > tmin)
					tmin = t1;

				if (tmin > tmax)
					return -1.0f;

			}
			else
			{
				if (-e - halfExtents.z > 0.0f || -e + halfExtents.z < 0.0f)
				{
					return -1.0f;
				}
			}
		}

		return tmin;
	}

	bool getPlanesIntersection(const Plane& p1, const Plane& p2, const Plane& p3, Vector3& ip)
	{
		const Vector3 n1 = p1.n;
		const Vector3 n2 = p2.n;
		const Vector3 n3 = p3.n;

		const float den = -Vector3Fn::dot(Vector3Fn::cross(n1, n2), n3);

		if (MathFn::equals(den, 0.0f))
		{
			return false;
		}

		const float invDen = 1.0f / den;

		Vector3 res = p1.d * Vector3Fn::cross(n2, n3) + p2.d * Vector3Fn::cross(n3, n1) + p3.d * Vector3Fn::cross(n1, n2);
		ip = res * invDen;

		return true;
	}

	bool getFrustumWithSphereIntersection(const Frustum& f, const Sphere& s)
	{
		if (PlaneFn::getDistanceToPoint(f.left, s.c) < -s.r ||
			PlaneFn::getDistanceToPoint(f.right, s.c) < -s.r)
		{
			return false;
		}

		if (PlaneFn::getDistanceToPoint(f.bottom, s.c) < -s.r ||
			PlaneFn::getDistanceToPoint(f.top, s.c) < -s.r)
		{
			return false;
		}

		if (PlaneFn::getDistanceToPoint(f.near, s.c) < -s.r ||
			PlaneFn::getDistanceToPoint(f.far, s.c) < -s.r)
		{
			return false;
		}

		return true;
	}

	bool getFrustumWithAaabIntersection(const Frustum& f, const Aabb& b)
	{
		uint8_t out;

		out = 0;
		if (PlaneFn::getDistanceToPoint(f.left, AabbFn::getVertexByIndex(b, 0)) < 0.0) out++;
		if (PlaneFn::getDistanceToPoint(f.left, AabbFn::getVertexByIndex(b, 1)) < 0.0) out++;
		if (PlaneFn::getDistanceToPoint(f.left, AabbFn::getVertexByIndex(b, 2)) < 0.0) out++;
		if (PlaneFn::getDistanceToPoint(f.left, AabbFn::getVertexByIndex(b, 3)) < 0.0) out++;
		if (PlaneFn::getDistanceToPoint(f.left, AabbFn::getVertexByIndex(b, 4)) < 0.0) out++;
		if (PlaneFn::getDistanceToPoint(f.left, AabbFn::getVertexByIndex(b, 5)) < 0.0) out++;
		if (PlaneFn::getDistanceToPoint(f.left, AabbFn::getVertexByIndex(b, 6)) < 0.0) out++;
		if (PlaneFn::getDistanceToPoint(f.left, AabbFn::getVertexByIndex(b, 7)) < 0.0) out++;

		// If all vertices are outside one face, then the box doesn't intersect the frustum
		if (out == 8) return false;

		out = 0;
		if (PlaneFn::getDistanceToPoint(f.right, AabbFn::getVertexByIndex(b, 0)) < 0.0) out++;
		if (PlaneFn::getDistanceToPoint(f.right, AabbFn::getVertexByIndex(b, 1)) < 0.0) out++;
		if (PlaneFn::getDistanceToPoint(f.right, AabbFn::getVertexByIndex(b, 2)) < 0.0) out++;
		if (PlaneFn::getDistanceToPoint(f.right, AabbFn::getVertexByIndex(b, 3)) < 0.0) out++;
		if (PlaneFn::getDistanceToPoint(f.right, AabbFn::getVertexByIndex(b, 4)) < 0.0) out++;
		if (PlaneFn::getDistanceToPoint(f.right, AabbFn::getVertexByIndex(b, 5)) < 0.0) out++;
		if (PlaneFn::getDistanceToPoint(f.right, AabbFn::getVertexByIndex(b, 6)) < 0.0) out++;
		if (PlaneFn::getDistanceToPoint(f.right, AabbFn::getVertexByIndex(b, 7)) < 0.0) out++;
		if (out == 8) return false;

		out = 0;
		if (PlaneFn::getDistanceToPoint(f.bottom, AabbFn::getVertexByIndex(b, 0)) < 0.0) out++;
		if (PlaneFn::getDistanceToPoint(f.bottom, AabbFn::getVertexByIndex(b, 1)) < 0.0) out++;
		if (PlaneFn::getDistanceToPoint(f.bottom, AabbFn::getVertexByIndex(b, 2)) < 0.0) out++;
		if (PlaneFn::getDistanceToPoint(f.bottom, AabbFn::getVertexByIndex(b, 3)) < 0.0) out++;
		if (PlaneFn::getDistanceToPoint(f.bottom, AabbFn::getVertexByIndex(b, 4)) < 0.0) out++;
		if (PlaneFn::getDistanceToPoint(f.bottom, AabbFn::getVertexByIndex(b, 5)) < 0.0) out++;
		if (PlaneFn::getDistanceToPoint(f.bottom, AabbFn::getVertexByIndex(b, 6)) < 0.0) out++;
		if (PlaneFn::getDistanceToPoint(f.bottom, AabbFn::getVertexByIndex(b, 7)) < 0.0) out++;
		if (out == 8) return false;

		out = 0;
		if (PlaneFn::getDistanceToPoint(f.top, AabbFn::getVertexByIndex(b, 0)) < 0.0) out++;
		if (PlaneFn::getDistanceToPoint(f.top, AabbFn::getVertexByIndex(b, 1)) < 0.0) out++;
		if (PlaneFn::getDistanceToPoint(f.top, AabbFn::getVertexByIndex(b, 2)) < 0.0) out++;
		if (PlaneFn::getDistanceToPoint(f.top, AabbFn::getVertexByIndex(b, 3)) < 0.0) out++;
		if (PlaneFn::getDistanceToPoint(f.top, AabbFn::getVertexByIndex(b, 4)) < 0.0) out++;
		if (PlaneFn::getDistanceToPoint(f.top, AabbFn::getVertexByIndex(b, 5)) < 0.0) out++;
		if (PlaneFn::getDistanceToPoint(f.top, AabbFn::getVertexByIndex(b, 6)) < 0.0) out++;
		if (PlaneFn::getDistanceToPoint(f.top, AabbFn::getVertexByIndex(b, 7)) < 0.0) out++;
		if (out == 8) return false;

		out = 0;
		if (PlaneFn::getDistanceToPoint(f.near, AabbFn::getVertexByIndex(b, 0)) < 0.0) out++;
		if (PlaneFn::getDistanceToPoint(f.near, AabbFn::getVertexByIndex(b, 1)) < 0.0) out++;
		if (PlaneFn::getDistanceToPoint(f.near, AabbFn::getVertexByIndex(b, 2)) < 0.0) out++;
		if (PlaneFn::getDistanceToPoint(f.near, AabbFn::getVertexByIndex(b, 3)) < 0.0) out++;
		if (PlaneFn::getDistanceToPoint(f.near, AabbFn::getVertexByIndex(b, 4)) < 0.0) out++;
		if (PlaneFn::getDistanceToPoint(f.near, AabbFn::getVertexByIndex(b, 5)) < 0.0) out++;
		if (PlaneFn::getDistanceToPoint(f.near, AabbFn::getVertexByIndex(b, 6)) < 0.0) out++;
		if (PlaneFn::getDistanceToPoint(f.near, AabbFn::getVertexByIndex(b, 7)) < 0.0) out++;
		if (out == 8) return false;

		out = 0;
		if (PlaneFn::getDistanceToPoint(f.far, AabbFn::getVertexByIndex(b, 0)) < 0.0) out++;
		if (PlaneFn::getDistanceToPoint(f.far, AabbFn::getVertexByIndex(b, 1)) < 0.0) out++;
		if (PlaneFn::getDistanceToPoint(f.far, AabbFn::getVertexByIndex(b, 2)) < 0.0) out++;
		if (PlaneFn::getDistanceToPoint(f.far, AabbFn::getVertexByIndex(b, 3)) < 0.0) out++;
		if (PlaneFn::getDistanceToPoint(f.far, AabbFn::getVertexByIndex(b, 4)) < 0.0) out++;
		if (PlaneFn::getDistanceToPoint(f.far, AabbFn::getVertexByIndex(b, 5)) < 0.0) out++;
		if (PlaneFn::getDistanceToPoint(f.far, AabbFn::getVertexByIndex(b, 6)) < 0.0) out++;
		if (PlaneFn::getDistanceToPoint(f.far, AabbFn::getVertexByIndex(b, 7)) < 0.0) out++;
		if (out == 8) return false;

		// If we are here, it is because either the box intersects or it is contained in the frustum
		return true;
	}

} // namespace Rio
