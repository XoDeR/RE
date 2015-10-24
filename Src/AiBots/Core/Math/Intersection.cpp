// Copyright (c) 2015 Volodymyr Syvochka
#include "Core/Math/Intersection.h"

#include "Core/Math/Vector3.h"
#include "Core/Math/Obb.h"
#include "Core/Math/Plane.h"
#include "Core/Math/Frustum.h"
#include "Core/Math/Sphere.h"

namespace Rio
{
	float getRayWithPlaneIntersection(const Vector3& from, const Vector3& dir, const Plane& p)
	{
		float nd = Vector3Fn::dot(dir, p.n);
		float orpn = Vector3Fn::dot(from, p.n);
		float dist = -1.0f;
		if (nd < 0.0f)
		{
			dist = (-p.d - orpn) / nd;
		}
		return dist > 0.0f ? dist : -1.0f;
	}

	float getRayWithSphereIntersection(const Vector3& from, const Vector3& dir, const Sphere& s)
	{
		Vector3 v = s.c - from;
		float b = Vector3Fn::dot(v, dir);
		float det = (s.r * s.r) - Vector3Fn::dot(v, v) + (b * b);
		if (det < 0.0 || b < s.r)
		{
			return -1.0f;
		}
		return b - sqrt(det);
	}

	float getRayWithOobIntersection(const Vector3& from, const Vector3& dir, const Obb& obb)
	{
		using namespace Vector3Fn;

		float tmin = 0.0f;
		float tmax = 100000.0f;

		Vector3 obb_pos(obb.tm.t.x, obb.tm.t.y, obb.tm.t.z);
		Vector3 delta = obb_pos - from;
		{
			const Vector3 xaxis(obb.tm.x.x, obb.tm.x.y, obb.tm.x.z);
			float e = dot(xaxis, delta);
			float f = dot(dir, xaxis);

			if (fabs(f) > 0.001f)
			{
				float t1 = (e + obb.aabb.min.x) / f;
				float t2 = (e + obb.aabb.max.x) / f;

				if (t1 > t2)
				{
					float w = t1; t1 = t2; t2 = w;
				}

				if (t2 < tmax)
					tmax = t2;
				if (t1 > tmin)
					tmin = t1;

				if (tmax < tmin)
					return -1.0f;
			}
			else
			{
				if (-e + obb.aabb.min.x > 0.0f || -e + obb.aabb.max.x < 0.0f)
					return -1.0f;
			}
		}

		{
			const Vector3 yaxis(obb.tm.y.x, obb.tm.y.y, obb.tm.y.z);
			float e = dot(yaxis, delta);
			float f = dot(dir, yaxis);

			if (fabs(f) > 0.001f){

				float t1 = (e + obb.aabb.min.y) / f;
				float t2 = (e + obb.aabb.max.y) / f;

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
				if (-e + obb.aabb.min.y > 0.0f || -e + obb.aabb.max.y < 0.0f)
					return -1.0f;
			}
		}

		{
			const Vector3 zaxis(obb.tm.z.x, obb.tm.z.y, obb.tm.z.z);
			float e = dot(zaxis, delta);
			float f = dot(dir, zaxis);

			if (fabs(f) > 0.001f)
			{

				float t1 = (e + obb.aabb.min.z) / f;
				float t2 = (e + obb.aabb.max.z) / f;

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
				if (-e + obb.aabb.min.z > 0.0f || -e + obb.aabb.max.z < 0.0f)
					return -1.0f;
			}
		}

	return tmin;
	}

	bool getPlanesIntersection(const Plane& p1, const Plane& p2, const Plane& p3, Vector3& ip)
	{
		const Vector3& n1 = p1.n;
		const Vector3& n2 = p2.n;
		const Vector3& n3 = p3.n;

		float den = -Vector3Fn::dot(Vector3Fn::cross(n1, n2), n3);

		if (equals(den, (float)0.0))
		{
			return false;
		}

		Vector3 res = p1.d * Vector3Fn::cross(n2, n3) + p2.d * Vector3Fn::cross(n3, n1) + p3.d * Vector3Fn::cross(n1, n2);
		ip = res / den;

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
