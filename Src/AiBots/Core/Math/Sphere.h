#pragma once

#include "Core/Base/Config.h"

#include "Core/Base/Types.h"

#include "Core/Math/MathUtils.h"
#include "Core/Math/Vector2.h"
#include "Core/Math/Vector3.h"
#include "Core/Math/Vector4.h"
#include "Core/Math/Quaternion.h"
#include "Core/Math/Matrix3x3.h"
#include "Core/Math/Matrix4x4.h"
#include "Core/Math/Aabb.h"
#include "Core/Math/Obb.h"
#include "Core/Math/Plane.h"
#include "Core/Math/Frustum.h"

namespace Rio
{

	struct Sphere
	{
		Sphere();
		Sphere(const Vector3& nc, float nr);
		Vector3 c;
		float r;
	};

	namespace sphere
	{
		float getVolume(const Sphere& s);
		// Adds num points to the sphere expanding if necessary
		void addPoints(Sphere& s, uint32_t num, const Vector3* points);
		// Adds num spheres expanding if necessary.
		void addSpheres(Sphere& s, uint32_t num, const Sphere* spheres);
		bool doesContainPoint(const Sphere& s, const Vector3& p);

		inline float getVolume(const Sphere& s)
		{
			return float(4.0 / 3.0 * PI) * s.r*s.r*s.r;
		}

		inline void addPoints(Sphere& s, uint32_t num, const Vector3* points)
		{
			for (uint32_t i = 0; i < num; ++i)
			{
				const float dist = Vector3Fn::getLengthSquared(points[i] - s.c);
				if (dist >= s.r*s.r)
					s.r = sqrt(dist);
			}
		}

		inline void addSpheres(Sphere& s, uint32_t num, const Sphere* spheres)
		{
			for (uint32_t i = 0; i < num; ++i)
			{
				const float dist = Vector3Fn::getLengthSquared(spheres[i].c - s.c);

				if (dist < (spheres[i].r + s.r) * (spheres[i].r + s.r))
				{
					if (spheres[i].r * spheres[i].r > s.r * s.r)
						s.r = sqrt(dist + spheres[i].r * spheres[i].r);
				}
			}
		}

		inline bool doesContainPoint(const Sphere& s, const Vector3& p)
		{
			float dist = Vector3Fn::getLengthSquared(p - s.c);
			return dist < s.r*s.r;
		}
	} // namespace sphere

	inline Sphere::Sphere()
	{
		// Do nothing
	}

	inline Sphere::Sphere(const Vector3& nc, float nr)
		: c(nc)
		, r(nr)
	{
	}

} // namespace Rio
