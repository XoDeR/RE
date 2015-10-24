#pragma once

#include "Core/Base/Config.h"

#include "Core/Base/Types.h"

namespace Rio
{

	//	Pseudo-random number generator.
	//	Uses LCG algorithm: fast and compatible with the standard C rand().
	struct Random
	{
		// Initializes the generator with the given seed.
		Random(int32_t seed);
		// Returns a pseudo-random integer in the range [0, 32767].
		int32_t getRandomInteger();
		// Returns a pseudo-random integer in the range [0, max).
		int32_t getRandomIntegerLessThanMax(int32_t max);
		// Returns a pseudo-random float in the range [0.0, 1.0].
		float getRandomUnitFloat();
	private:
		int32_t seed;
	};

	inline Random::Random(int32_t seed) 
		: seed(seed)
	{
	}

	inline int32_t Random::getRandomInteger()
	{
		seed = 214013 * seed + 13737667;
		return (seed >> 16) & 0x7FFF;
	}

	inline int32_t Random::getRandomIntegerLessThanMax(int32_t max)
	{
		return (max == 0) ? 0 : getRandomInteger() % max;
	}

	inline float Random::getRandomUnitFloat()
	{
		return getRandomInteger() / (float)0x7FFF;
	}

} // namespace Rio
