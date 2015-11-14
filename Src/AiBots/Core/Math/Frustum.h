// Copyright (c) 2015 Volodymyr Syvochka
#pragma once

#include "Core/Base/Config.h"

#include "Core/Base/Types.h"

#include "Core/Math/Vector2.h"
#include "Core/Math/Vector3.h"
#include "Core/Math/Vector4.h"
#include "Core/Math/Quaternion.h"
#include "Core/Math/Matrix3x3.h"
#include "Core/Math/Matrix4x4.h"
#include "Core/Math/Aabb.h"
#include "Core/Math/Obb.h"
#include "Core/Math/Plane.h"
#include "Core/Math/Sphere.h"
#include "Core/Math/Intersection.h"

namespace Rio
{
	struct Frustum
	{
		Frustum();
		Plane left;
		Plane right;
		Plane bottom;
		Plane top;
		Plane near;
		Plane far;
	};

	inline Frustum::Frustum()
	{
		// Do not initialize
	}

	namespace FrustumFn
	{
		// Returns whether the frustum contains the point
		bool doesContainPoint(const Frustum& f, const Vector3& p);

		// Returns the corner index of the frustum 
		// Index to corner table:
		// 0 = Near bottom left
		// 1 = Near bottom right
		// 2 = Near top right
		// 3 = Near top left
		// 4 = Far bottom left
		// 5 = Far bottom right
		// 6 = Far top right
		// 7 = Far top left
		Vector3 getVertexByIndex(const Frustum& f, uint32_t index);

		// Builds the frustum from the view matrix
		void createFromMatrix(Frustum& f, const Matrix4x4& m);
		// Returns the Aabb enclosing the frustum
		Aabb toAabb(const Frustum& f);

		inline bool doesContainPoint(const Frustum& f, const Vector3& p)
		{
			if (PlaneFn::getDistanceToPoint(f.left, p) < 0.0) return false;
			if (PlaneFn::getDistanceToPoint(f.right, p) < 0.0) return false;
			if (PlaneFn::getDistanceToPoint(f.bottom, p) < 0.0) return false;
			if (PlaneFn::getDistanceToPoint(f.top, p) < 0.0) return false;
			if (PlaneFn::getDistanceToPoint(f.near, p) < 0.0) return false;
			if (PlaneFn::getDistanceToPoint(f.far, p) < 0.0) return false;

			return true;
		}

		inline Vector3 getVertexByIndex(const Frustum& f, uint32_t index)
		{
			RIO_ASSERT(index < 8, "Index must be < 8");

			// 0 = Near bottom left
			// 1 = Near bottom right
			// 2 = Near top right
			// 3 = Near top left
			// 4 = Far bottom left
			// 5 = Far bottom right
			// 6 = Far top right
			// 7 = Far top left

			const Plane* side = &f.left;
			Vector3 ip;

			switch (index)
			{
			case 0: return getPlanesIntersection(side[4], side[0], side[2], ip);
			case 1: return getPlanesIntersection(side[4], side[1], side[2], ip);
			case 2: return getPlanesIntersection(side[4], side[1], side[3], ip);
			case 3: return getPlanesIntersection(side[4], side[0], side[3], ip);
			case 4: return getPlanesIntersection(side[5], side[0], side[2], ip);
			case 5: return getPlanesIntersection(side[5], side[1], side[2], ip);
			case 6: return getPlanesIntersection(side[5], side[1], side[3], ip);
			case 7: return getPlanesIntersection(side[5], side[0], side[3], ip);
			default: break;
			}

			return ip;
		}

		inline void createFromMatrix(Frustum& f, const Matrix4x4& m)
		{
			f.left.n.x = m[3] + m[0];
			f.left.n.y = m[7] + m[4];
			f.left.n.z = m[11] + m[8];
			f.left.d = m[15] + m[12];

			f.right.n.x = m[3] - m[0];
			f.right.n.y = m[7] - m[4];
			f.right.n.z = m[11] - m[8];
			f.right.d = m[15] - m[12];

			f.bottom.n.x = m[3] + m[1];
			f.bottom.n.y = m[7] + m[5];
			f.bottom.n.z = m[11] + m[9];
			f.bottom.d = m[15] + m[13];

			f.top.n.x = m[3] - m[1];
			f.top.n.y = m[7] - m[5];
			f.top.n.z = m[11] - m[9];
			f.top.d = m[15] - m[13];

			f.near.n.x = m[3] + m[2];
			f.near.n.y = m[7] + m[6];
			f.near.n.z = m[11] + m[10];
			f.near.d = m[15] + m[14];

			f.far.n.x = m[3] - m[2];
			f.far.n.y = m[7] - m[6];
			f.far.n.z = m[11] - m[10];
			f.far.d = m[15] - m[14];

			PlaneFn::normalize(f.left);
			PlaneFn::normalize(f.right);
			PlaneFn::normalize(f.bottom);
			PlaneFn::normalize(f.top);
			PlaneFn::normalize(f.near);
			PlaneFn::normalize(f.far);
		}

		inline Aabb toAabb(const Frustum& f)
		{
			Aabb tmp;
			AabbFn::reset(tmp);

			Vector3 vertices[8];
			vertices[0] = getVertexByIndex(f, 0);
			vertices[1] = getVertexByIndex(f, 1);
			vertices[2] = getVertexByIndex(f, 2);
			vertices[3] = getVertexByIndex(f, 3);
			vertices[4] = getVertexByIndex(f, 4);
			vertices[5] = getVertexByIndex(f, 5);
			vertices[6] = getVertexByIndex(f, 6);
			vertices[7] = getVertexByIndex(f, 7);

			AabbFn::addPoints(tmp, 8, vertices);

			return tmp;
		}
	} // namespace FrustumFn

} // namespace Rio
