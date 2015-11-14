#include "Core/Math/Matrix4x4.h"
#include "Core/Math/MathFn.h"

namespace Rio
{
	const Matrix4x4 Matrix4x4::Identity = 
	{
		Vector4{ 1, 0, 0, 0 },
		Vector4{ 0, 1, 0, 0 },
		Vector4{ 0, 0, 1, 0 },
		Vector4{ 0, 0, 0, 1 } 
	};

	namespace Matrix4x4Fn
	{
		//******************************
		// Transformation
		//******************************
		Matrix4x4 translate(const Vector3& v)
		{
			Matrix4x4 result = Matrix4x4::Identity;
			result.data[3].xyz = v;
			result.data[3].w = 1;
			return result;
		}

		// Angle in Radians
		Matrix4x4 rotate(const Radian& angle, const Vector3& v)
		{
			const float c = MathFn::cos(angle);
			const float s = MathFn::sin(angle);

			const Vector3 axis = getNormalized(v);
			const Vector3 t = (1.0f - c) * axis;

			Matrix4x4 rot = Matrix4x4::Identity;
			rot.data[0][0] = c + t.x * axis.x;
			rot.data[0][1] = 0 + t.x * axis.y + s * axis.z;
			rot.data[0][2] = 0 + t.x * axis.z - s * axis.y;
			rot.data[0][3] = 0;

			rot.data[1][0] = 0 + t.y * axis.x - s * axis.z;
			rot.data[1][1] = c + t.y * axis.y;
			rot.data[1][2] = 0 + t.y * axis.z + s * axis.x;
			rot.data[1][3] = 0;

			rot.data[2][0] = 0 + t.z * axis.x + s * axis.y;
			rot.data[2][1] = 0 + t.z * axis.y - s * axis.x;
			rot.data[2][2] = c + t.z * axis.z;
			rot.data[2][3] = 0;

			return rot;
		}

		Matrix4x4 scale(const Vector3& v)
		{
			return Matrix4x4{ Vector4{ v.x, 0, 0, 0 },
				Vector4{ 0, v.y, 0, 0 },
				Vector4{ 0, 0, v.z, 0 },
				Vector4{ 0, 0, 0, 1 } };
		}

		Matrix4x4 ortho(float left, float right, float bottom, float top)
		{
			Matrix4x4 result = Matrix4x4::Identity;

			result.data[0][0] = 2.0f / (right - left);
			result.data[1][1] = 2.0f / (top - bottom);
			result.data[2][2] = -1.0f;
			result.data[3][1] = -(right + left) / (right - left);
			result.data[3][1] = -(top + bottom) / (top - bottom);

			return result;
		}

		Matrix4x4 ortho(float left, float right, float bottom, float top, float zNear, float zFar)
		{
			Matrix4x4 result = Matrix4x4::Identity;

			result.data[0][0] = 2.0f / (right - left);
			result.data[1][1] = 2.0f / (top - bottom);
			result.data[2][2] = -2.0f / (zFar - zNear);
			result.data[3][0] = -(right + left) / (right - left);
			result.data[3][1] = -(top + bottom) / (top - bottom);
			result.data[3][2] = -(zFar + zNear) / (zFar - zNear);

			return result;
		}

		Matrix4x4 perspective(const Radian& fovy, float aspect, float zNear, float zFar)
		{
			RIO_ASSERT(MathFn::abs(aspect - std::numeric_limits<float>::epsilon()) > 0.0f,
				"MathFn::perspective fovy is 0/inf.");

			const float tanHalfFovy = MathFn::tan(0.5f * fovy);

			Matrix4x4 result = {};
			result.data[0][0] = 1.0f / (aspect * tanHalfFovy);
			result.data[1][1] = 1.0f / (tanHalfFovy);
			result.data[2][2] = -(zFar + zNear) / (zFar - zNear);
			result.data[2][3] = -1.0f;
			result.data[3][2] = -2.0f * zFar * zNear / (zFar - zNear);

			return result;
		}

		Matrix4x4 infinitePerspective(const Radian& fovy, float aspect, float zNear)
		{
			const float range = MathFn::tan(0.5f * fovy) * zNear;
			const float left = -range * aspect;
			const float right = range * aspect;
			const float bottom = -range;
			const float top = range;

			Matrix4x4 result = {};

			result.data[0][0] = (2.0f * zNear) / (right - left);
			result.data[1][1] = (2.0f * zNear) / (top - bottom);
			result.data[2][2] = -1.0f;
			result.data[2][3] = -1.0f;
			result.data[3][2] = -2.0f * zNear;

			return result;
		}

		Matrix4x4 lookAtMatrix4(const Vector3& eye, const Vector3& center, const Vector3& up)
		{
			const Vector3 f = Vector3Fn::normalize(center - eye);
			const Vector3 s = Vector3Fn::normalize(Vector3Fn::cross(f, up));
			const Vector3 u = Vector3Fn::cross(s, f);

			Matrix4x4 result = Matrix4x4::Identity;
			result.data[0][0] = +s.x;
			result.data[1][0] = +s.y;
			result.data[2][0] = +s.z;

			result.data[0][1] = +u.x;
			result.data[1][1] = +u.y;
			result.data[2][1] = +u.z;

			result.data[0][2] = -f.x;
			result.data[1][2] = -f.y;
			result.data[2][2] = -f.z;

			result.data[3][0] = -Vector3Fn::dot(s, eye);
			result.data[3][1] = -Vector3Fn::dot(u, eye);
			result.data[3][2] = +Vector3Fn::dot(f, eye);

			return result;
		}

		// TODO: properly implement quaternionLookAt
		Quaternion lookAtQuaternion(const Vector3& eye, const Vector3& center, const Vector3& up)
		{
			const float similar = 0.001f;

			if (Vector3Fn::getLength(center - eye) < similar)
			{
				return Quaternion::Identity; // You cannot look at where you are!
			}

			return Matrix4x4Fn::matrix4x4ToQuaternion(lookAtMatrix4(eye, center, up));

			// const Vector3 f{normalize(center - eye)};
			// const Vector3 s{normalize(cross(f, up))};
			// const Vector3 u{cross(s, f)};
			// const Vector3 refUp{normalize(up)};

			//// NOTE: this is from
			////
			/// http://lolengine.net/blog/2013/09/18/beautiful-maths-quaternion-from-vectors
			// const float m{MathFn::sqrt(2.0f + 2.0f * dot(u, refUp))};
			// Vector3 v{(1.0f / m) * cross(u, refUp)};
			// return {v, 0.5f * m};
		}

		// NOTE: Assumes matrix is only a rotational matrix and has no shear applied
		Quaternion matrix4x4ToQuaternion(const Matrix4x4& m)
		{
			const float fourXSquaredMinus1 = m[0 * 4 + 0] - m[1 * 4 + 1] - m[2 * 4 + 2];
			const float fourYSquaredMinus1 = m[1 * 4 + 1] - m[0 * 4 + 0] - m[2 * 4 + 2];
			const float fourZSquaredMinus1 = m[2 * 4 + 2] - m[0 * 4 + 0] - m[1 * 4 + 1];
			const float fourWSquaredMinus1 = m[0 * 4 + 0] + m[1 * 4 + 1] + m[2 * 4 + 2];

			int32_t biggestIndex = 0;
			float fourBiggestSquaredMinus1{ fourWSquaredMinus1 };
			if (fourXSquaredMinus1 > fourBiggestSquaredMinus1)
			{
				fourBiggestSquaredMinus1 = fourXSquaredMinus1;
				biggestIndex = 1;
			}
			if (fourYSquaredMinus1 > fourBiggestSquaredMinus1)
			{
				fourBiggestSquaredMinus1 = fourYSquaredMinus1;
				biggestIndex = 2;
			}
			if (fourZSquaredMinus1 > fourBiggestSquaredMinus1)
			{
				fourBiggestSquaredMinus1 = fourZSquaredMinus1;
				biggestIndex = 3;
			}

			const float biggestVal = MathFn::sqrt(fourBiggestSquaredMinus1 + 1.0f) * 0.5f;
			const float mult = 0.25f / biggestVal;

			Quaternion q = Quaternion{ 0, 0, 0, 1 };

			switch (biggestIndex)
			{
			case 0:
			{
				q.w = biggestVal;
				q.x = (m[1 * 4 + 2] - m[2 * 4 + 1]) * mult;
				q.y = (m[2 * 4 + 0] - m[0 * 4 + 2]) * mult;
				q.z = (m[0 * 4 + 1] - m[1 * 4 + 0]) * mult;
			}
			break;
			case 1:
			{
				q.w = (m[1 * 4 + 2] - m[2 * 4 + 1]) * mult;
				q.x = biggestVal;
				q.y = (m[0 * 4 + 1] + m[1 * 4 + 0]) * mult;
				q.z = (m[2 * 4 + 0] + m[0 * 4 + 2]) * mult;
			}
			break;
			case 2:
			{
				q.w = (m[2 * 4 + 0] - m[0 * 4 + 2]) * mult;
				q.x = (m[0 * 4 + 1] + m[1 * 4 + 0]) * mult;
				q.y = biggestVal;
				q.z = (m[1 * 4 + 2] + m[2 * 4 + 1]) * mult;
			}
			break;
			case 3:
			{
				q.w = (m[0 * 4 + 1] - m[1 * 4 + 0]) * mult;
				q.x = (m[2 * 4 + 0] + m[0 * 4 + 2]) * mult;
				q.y = (m[1 * 4 + 2] + m[2 * 4 + 1]) * mult;
				q.z = biggestVal;
			}
			break;
			default: // Should never actually get here
			{
				RIO_ASSERT(false, "How did you get here?!");
			}
			break;
			}

			return q;
		}

	} // namespace Matrix4x4Fn

} // namespace  Rio
