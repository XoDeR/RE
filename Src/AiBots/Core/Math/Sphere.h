#pragma once

#include "Core/Base/Config.h"

#include "Core/Base/Types.h"

#include "Core/Math/MathFn.h"
#include "Core/Math/Vector3.h"

namespace Rio
{
	struct Sphere
	{
		Sphere();
		Sphere(const Vector3& center, float radius);
		Vector3 c;
		float r;
	};

	namespace SphereFn
	{
		void reset(Sphere& s);
		float getVolume(const Sphere& s);
		// Adds num points to the sphere expanding if necessary
		void addPoints(Sphere& s, uint32_t num, uint32_t stride, const void* points);
		void addPoints(Sphere& s, uint32_t num, const Vector3* points);
		// Adds num spheres expanding if necessary.
		void addSpheres(Sphere& s, uint32_t num, const Sphere* spheres);
		bool doesContainPoint(const Sphere& s, const Vector3& p);

		inline void reset(Sphere& s)
		{
			s.c = Vector3::Zero;
			s.r = 0.0f;
		}

		inline float getVolume(const Sphere& s)
		{
			return float(4.0 / 3.0 * MathFn::Pi) * s.r*s.r*s.r;
		}

		inline void addPoints(Sphere& s, uint32_t num, uint32_t stride, const void* points)
		{
			for (uint32_t i = 0; i < num; ++i)
			{
				const Vector3* p = (const Vector3*)points;

				const float dist = Vector3Fn::getLengthSquared(*p - s.c);
				if (dist > s.r*s.r)
				{
					s.r = MathFn::sqrt(dist);
				}

				points = (const void*)((const char*)points + stride);
			}
		}

		inline void addPoints(Sphere& s, uint32_t num, const Vector3* points)
		{
			addPoints(s, num, sizeof(Vector2), points);
		}

		inline void addSpheres(Sphere& s, uint32_t num, const Sphere* spheres)
		{
			for (uint32_t i = 0; i < num; ++i)
			{
				const float dist = Vector3Fn::getLengthSquared(spheres[i].c - s.c);

				if (dist < (spheres[i].r + s.r) * (spheres[i].r + s.r))
				{
					if (spheres[i].r * spheres[i].r > s.r * s.r)
					{
						s.r = MathFn::sqrt(dist + spheres[i].r * spheres[i].r);
					}
				}
			}
		}

		inline bool doesContainPoint(const Sphere& s, const Vector3& p)
		{
			float dist = Vector3Fn::getLengthSquared(p - s.c);
			return dist < s.r*s.r;
		}
	} // namespace SphereFn

	inline Sphere::Sphere()
	{
		// Do nothing
	}

	inline Sphere::Sphere(const Vector3& center, float radius)
		: c(center)
		, r(radius)
	{
	}

} // namespace Rio
