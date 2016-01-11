#pragma once

#include "Core/Base/Types.h"
#include "Core/Math/Unit.h"
#include "Core/Math/MathConstants.h"

namespace Rio
{
	namespace Impl
	{
		using namespace MathFn;

		template <typename T>
		class Radian;

		// Degree
		template <typename T>
		class Degree : public Unit<Degree, T>
		{
		public:
			Degree() {}

			explicit Degree(T value)
				: Unit<Rio::Impl::Degree, T>(value)
			{
			}

			Degree(Unit<Rio::Impl::Degree, T> value)
				: Unit<Rio::Impl::Degree, T>(value)
			{
			}

			template <class U>
			explicit Degree(Unit<Rio::Impl::Degree, U> value)
				: Unit<Rio::Impl::Degree, T>(value)
			{
			}

			Degree(Unit<Radian, T> value);
		};

		// Radian
		template <typename T>
		class Radian : public Unit<Radian, T>
		{
		public:
			Radian() {}

			explicit Radian(T value)
				: Unit<Rio::Impl::Radian, T>(value)
			{
			}

			Radian(Unit<Rio::Impl::Radian, T> value)
				: Unit<Rio::Impl::Radian, T>(value)
			{
			}
			template <class U>
			explicit Radian(Unit<Rio::Impl::Radian, U> value)
				: Unit<Rio::Impl::Radian, T>(value)
			{
			}

			Radian(Unit<Degree, T> value);
		};

		template <typename T>
		Degree<T>::Degree(Unit<Radian, T> value)
			: Unit<Rio::Impl::Degree, T>(T(360) * T(value) / MathFn::Tau)
		{
		}

		template <typename T>
		Radian<T>::Radian(Unit<Degree, T> value)
			: Unit<Rio::Impl::Radian, T>(T(value) * MathFn::Tau / T(360))
		{
		}
	} // namespace Impl

	using Radian = Impl::Radian<float>;
	using Degree = Impl::Degree<float>;

} // namespace Rio
