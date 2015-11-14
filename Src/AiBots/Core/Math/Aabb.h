// Copyright (c) 2015 Volodymyr Syvochka
#pragma once

#include "Core/Base/Config.h"

#include "Core/Debug/Error.h"
#include "Core/Base/Types.h"

#include "Core/Math/Vector3.h"
#include "Core/Math/Vector4.h"
#include "Core/Math/Quaternion.h"
#include "Core/Math/Matrix3x3.h"
#include "Core/Math/Matrix4x4.h"
#include "Core/Math/Sphere.h"

namespace Rio
{
	struct Aabb
	{
		Aabb();
		// Constructs from min and max.
		Aabb(const Vector3& min, const Vector3& max);
		Vector3 min;
		Vector3 max;
	};

	inline Aabb::Aabb()
	{
		// Do not initialize
	}

	inline Aabb::Aabb(const Vector3& min, const Vector3& max)
		: min(min)
		, max(max)
	{
	}

	namespace AabbFn
	{
		void reset(Aabb& b);
		Vector3 getCenter(const Aabb& b);
		float getRadius(const Aabb& b);
		float getVolume(const Aabb& b);
		// Adds num points to the box, expanding its bounds if necessary
		void addPoints(Aabb& b, uint32_t num, const Vector3* points);
		// Adds num boxes to the box, expanding its bounds if necessary.
		void addBoxes(Aabb& b, uint32_t num, const Aabb* boxes);
		// Returns whether box contains point
		bool doesContainPoint(const Aabb& b, const Vector3& p);
		// Returns the index-th vertex of the box.
		Vector3 getVertexByIndex(const Aabb& b, uint32_t index);
		// Returns the box enclosing b transformed by m
		Aabb getTransformed(const Aabb& b, const Matrix4x4& m);
		// Returns the eight vertices of the box b
		void toVertices(const Aabb& b, Vector3 v[8]);
		// Returns the sphere enclosing the box b
		Sphere toSphere(const Aabb& b);

		inline void reset(Aabb& b)
		{
			b.min = Vector3::Zero;
			b.max = Vector3::Zero;
		}

		inline Vector3 getCenter(const Aabb& b)
		{
			return (b.min + b.max) * 0.5;
		}

		inline float getRadius(const Aabb& b)
		{
			return Vector3Fn::getLength(b.max - (b.min + b.max) * 0.5);
		}
		inline float getVolume(const Aabb& b)
		{
			return (b.max.x - b.min.x) * (b.max.y - b.min.y) * (b.max.z - b.min.z);
		}

		inline void addPoints(Aabb& b, uint32_t num, const Vector3* points)
		{
			for (uint32_t i = 0; i < num; i++)
			{
				const Vector3& p = points[i];

				if (p.x < b.min.x) b.min.x = p.x;
				if (p.y < b.min.y) b.min.y = p.y;
				if (p.z < b.min.z) b.min.z = p.z;
				if (p.x > b.max.x) b.max.x = p.x;
				if (p.y > b.max.y) b.max.y = p.y;
				if (p.z > b.max.z) b.max.z = p.z;
			}
		}

		inline void addBoxes(Aabb& b, uint32_t num, const Aabb* boxes)
		{
			for (uint32_t i = 0; i < num; i++)
			{
				const Aabb& box = boxes[i];

				if (box.min.x < b.min.x) b.min.x = box.min.x;
				if (box.min.y < b.min.y) b.min.y = box.min.y;
				if (box.min.z < b.min.z) b.min.z = box.min.z;
				if (box.max.x > b.max.x) b.max.x = box.max.x;
				if (box.max.y > b.max.y) b.max.y = box.max.y;
				if (box.max.z > b.max.z) b.max.z = box.max.z;
			}
		}

		inline bool doesContainPoint(const Aabb& b, const Vector3& p)
		{
			return (p.x > b.min.x &&
				p.y > b.min.y &&
				p.z > b.min.z &&
				p.x < b.max.x &&
				p.y < b.max.y &&
				p.z < b.max.z);
		}

		inline Vector3 getVertexByIndex(const Aabb& b, uint32_t index)
		{
			switch (index)
			{
				case 0: return Vector3(b.min.x, b.min.y, b.min.z);
				case 1: return Vector3(b.max.x, b.min.y, b.min.z);
				case 2: return Vector3(b.max.x, b.min.y, b.max.z);
				case 3: return Vector3(b.min.x, b.min.y, b.max.z);
				case 4: return Vector3(b.min.x, b.max.y, b.min.z);
				case 5: return Vector3(b.max.x, b.max.y, b.min.z);
				case 6: return Vector3(b.max.x, b.max.y, b.max.z);
				case 7: return Vector3(b.min.x, b.max.y, b.max.z);
				default: RIO_FATAL("Bad index"); return Vector3(0.0f, 0.0f, 0.0f);
			}
		}

		inline Aabb getTransformed(const Aabb& b, const Matrix4x4& m)
		{
			Vector3 vertices[8];
			toVertices(b, vertices);

			vertices[0] = vertices[0] * m;
			vertices[1] = vertices[1] * m;
			vertices[2] = vertices[2] * m;
			vertices[3] = vertices[3] * m;
			vertices[4] = vertices[4] * m;
			vertices[5] = vertices[5] * m;
			vertices[6] = vertices[6] * m;
			vertices[7] = vertices[7] * m;

			Aabb res;
			reset(res);
			addPoints(res, 8, vertices);
			return res;
		}

		inline void toVertices(const Aabb& b, Vector3 v[8])
		{
			// 7 ---- 6
			// |      |
			// |      |  <-- Top face
			// 4 ---- 5
			//
			// 3 ---- 2
			// |      |
			// |      |  <-- Bottom face
			// 0 ---- 1
			v[0].x = b.min.x;
			v[0].y = b.min.y;
			v[0].z = b.max.z;

			v[1].x = b.max.x;
			v[1].y = b.min.y;
			v[1].z = b.max.z;

			v[2].x = b.max.x;
			v[2].y = b.min.y;
			v[2].z = b.min.z;

			v[3].x = b.min.x;
			v[3].y = b.min.y;
			v[3].z = b.min.z;

			v[4].x = b.min.x;
			v[4].y = b.max.y;
			v[4].z = b.max.z;

			v[5].x = b.max.x;
			v[5].y = b.max.y;
			v[5].z = b.max.z;

			v[6].x = b.max.x;
			v[6].y = b.max.y;
			v[6].z = b.min.z;

			v[7].x = b.min.x;
			v[7].y = b.max.y;
			v[7].z = b.min.z;
		}

		inline Sphere toSphere(const Aabb& b)
		{
			return Sphere(getCenter(b), getRadius(b));
		}
	} // namespace AabbFn
} // namespace Rio
