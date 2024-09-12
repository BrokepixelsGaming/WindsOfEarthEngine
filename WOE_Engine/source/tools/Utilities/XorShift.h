#pragma once

#include <stdint.h>
#include <concepts>
#include <type_traits>
#include "Vector.h"



// For now I am keeping this solely in the header file. If this grows beyond a hundred lines I will
// Break it down into two files. but for now it will stay like this. (current 67 lines)
class XorShift
{

private:

	// [0] will be used to store the current state. 
	// [1] will be the constant. I will guarentee this is a value I set if I ResetState() or a new RNG regardless if seeded or not. 
	// [2] will be the stored seed. This is meant for debugging and if there is a reason to reset the seed. 
	uint64_t m_State[3];

	uint64_t Random();
	float TwoDSqrt2Over4;
public:

	// If no seed is applied time will be used. 
	XorShift(uint64_t seed = 0);

	// Set the seed for the generator. Zero is not valid and will not set the seed. 
	void SetSeed(uint64_t seed);

	// Used to reset the state back to it's initial non used state. (This is expected to be only used with debugging)
	void ResetState();

	bool RandomPercentChance(double chance);
	bool RandomBool();

	// Generate a random integer between low and high (inclusive)
	template<std::integral Type>
	Type RandomInRange(Type low, Type high);

	//float RandomInRange(float low,float high);
	unsigned int Get1DNoise(int x, unsigned int seedOverride) noexcept;
	unsigned int Get2DNoise(int x, int y, unsigned int seedOverride) noexcept;
	unsigned int Get3DNoise(int x, int y, int z, unsigned int seedOverride) noexcept;
	Vector2<float> GetRandomGradientUsingSquirrelNoise(int x, int y, unsigned int seedOverride);
	float DotGridGradient(int ix, int iy, float x, float y, unsigned int seedOverride);
	float Interpolate(float a0, float a1, float weight);
	float Perlin(float x, float y, unsigned int seedOverride);
	float NormalizeNoise(unsigned int noise);
	float RandomNormalizedValue();
	uint64_t GetCurrentSeedUInt64()const { return m_State[2]; }
	unsigned int GetCurrentSeedUInt()const { return static_cast<unsigned int>(m_State[2]); }
	void ChangeSeedRandomly();
};

template<std::integral Type>
inline Type XorShift::RandomInRange(Type low, Type high)
{
	uint64_t r = Random();
	return static_cast<Type>(low + (r % (high - low + 1)));
}
