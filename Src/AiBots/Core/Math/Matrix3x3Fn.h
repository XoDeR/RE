#include "Core/Math/Matrix3x3.h"
#include "Core/Math/Matrix4x4.h"

namespace Rio
{
	namespace Matrix3x3Fn
	{
		// Returns a 4x4 matrix according to the matrix's rotation portion
		Matrix4x4 toMatrix4x4(const Matrix3x3& m);

		inline Matrix4x4 toMatrix4x4(const Matrix3x3& m)
		{
			return Matrix4x4(m);
		}
	} // namespace Matrix3x3Fn
} // namespace Rio