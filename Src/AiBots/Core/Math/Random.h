#pragma once

#include "Core/Base/Config.h"

#include "Core/Base/Types.h"
#include <random>

namespace Rio
{
	//	Pseudo-random number generator.
	//	Uses LCG (Linear congruential generator) algorithm: fast and compatible with the standard C rand().
	struct RandomLcg
	{
		// Initializes the generator with the given seed.
		RandomLcg(int32_t seed);
		// Returns a pseudo-random integer in the range [0, 32767].
		int32_t getRandomInteger();
		// Returns a pseudo-random integer in the range [0, max).
		int32_t getRandomIntegerLessThanMax(int32_t max);
		// Returns a pseudo-random float in the range [0.0, 1.0].
		float getRandomUnitFloat();
	private:
		int32_t seed;
	};

	inline RandomLcg::RandomLcg(int32_t seed)
		: seed(seed)
	{
	}

	inline int32_t RandomLcg::getRandomInteger()
	{
		seed = 214013 * seed + 13737667;
		return (seed >> 16) & 0x7FFF;
	}

	inline int32_t RandomLcg::getRandomIntegerLessThanMax(int32_t max)
	{
		return (max == 0) ? 0 : getRandomInteger() % max;
	}

	inline float RandomLcg::getRandomUnitFloat()
	{
		return getRandomInteger() / (float)0x7FFF;
	}

	class RandomStd
	{
	public:
		using ResultType = std::mt19937::result_type;

		RandomStd() = default;

		RandomStd(ResultType seed)
			: randomEngine{ seed }
		{
		}

		void setSeed(ResultType seed)
		{
			randomEngine.seed(seed);
		}

		// inclusive
		int32_t getInt(int32_t min, int32_t max)
		{
			return std::uniform_int_distribution < int32_t > {min, max}(randomEngine);
		}

		// inclusive
		float getFloat(float min, float max)
		{
			return std::uniform_real_distribution < float > {min, max}(randomEngine);
		}

		bool getBool()
		{
			return getInt(0, 1) == 1;
		}
	private:
		// A Mersenne Twister pseudo - random generator of 32 - bit numbers with a state size of 19937 bits.
		std::mt19937 randomEngine = std::mt19937(std::random_device{}());
	};

} // namespace Rio
