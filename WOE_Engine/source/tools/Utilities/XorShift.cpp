#include "XorShift.h"
#include <assert.h>
#include <time.h>
#include <cmath>



uint64_t XorShift::Random()
{
	uint64_t x = m_State[0];
	uint64_t y = m_State[1];

	m_State[0] = y;
	x ^= x << 23;
	x ^= x >> 18;
	x ^= y ^ (y >> 5);
	m_State[1] = x;

	return x + y;
}

XorShift::XorShift(uint64_t seed)
{
	TwoDSqrt2Over4 = sqrt(.5f);
	// Check to ensure seed isn't zero. If so we use time to generate seed. 
	if (seed == 0)
	{
		// Default seed if not provided
		seed = static_cast<uint64_t>(time(nullptr));
	}

	m_State[0] = seed;
	m_State[1] = 0x6C078965; // prime number 1,812,433,253
	m_State[2] = seed;
}

void XorShift::SetSeed(uint64_t seed)
{
	assert(seed != 0); // we will break if we are forcing a zero. 

	m_State[0] = seed;
	m_State[1] = 0x6C078965; // prime number 1,812,433,253
	m_State[2] = seed;
}

void XorShift::ResetState()
{
	m_State[0] = m_State[2];
	m_State[1] = 0x6C078965; // prime number 1,812,433,253
}

bool XorShift::RandomPercentChance(double chance)
{
	double randomChance = static_cast<double>(Random()) / static_cast<double> (0xFFFFFFFFFFFFFFFF);
	if (chance > randomChance)
		return true;
	return false;
}

bool XorShift::RandomBool()
{
	return RandomPercentChance(0.50);
}


unsigned int XorShift::Get1DNoise(int x, unsigned int seedOverride) noexcept
{
	// This algorithm (called SquirrelNoise3) was originally written by Squirrel Eiserloh and was presented in 
	// his 2017 GDC talk titled Math for Game Programmers: Noise-Based RNG, which can be found here:
	// https://www.gdcvault.com/play/1024365/Math-for-Game-Programmers-Noise

	constexpr unsigned int kBitNoise1 = 0x68e31da4;
	constexpr unsigned int kBitNoise2 = 0xb5297a4d;
	constexpr unsigned int kBitNoise3 = 0x1b56c4e9;

	unsigned int mangledBits = static_cast<unsigned int>(x);
	mangledBits *= kBitNoise1;
	mangledBits += seedOverride;
	mangledBits ^= (mangledBits >> 8);
	mangledBits *= kBitNoise2;
	mangledBits ^= (mangledBits << 8);
	mangledBits *= kBitNoise3;
	mangledBits ^= (mangledBits >> 8);
	return mangledBits;
}

unsigned int XorShift::Get2DNoise(int x, int y, unsigned int seedOverride) noexcept
{
	constexpr unsigned int kPrime = 198491317;
	return Get1DNoise(x + (kPrime * y), seedOverride);
}

unsigned int XorShift::Get3DNoise(int x, int y, int z, unsigned int seedOverride) noexcept
{
	constexpr unsigned int kPrime1 = 198491317;
	constexpr unsigned int kPrime2 = 6543989;
	return Get1DNoise(x + (kPrime1 * y) + (kPrime2 * z), seedOverride);
}

Vector2<float> XorShift::GetRandomGradientUsingSquirrelNoise(int x, int y, unsigned int seedOverride)
{
	float random = static_cast<float>(Get2DNoise(x, y, seedOverride) * (kDoublePI / ~(~0u >> 1))); // in [0, 2*Pi]
	return Vector2<float>{cos(random), sin(random)};
}

float XorShift::DotGridGradient(int iX, int iY, float x, float y, unsigned int seedOverride)
{
	// Get gradient from integer coordinates
	auto gradient = GetRandomGradientUsingSquirrelNoise(iX, iY, seedOverride);

	// Compute the distance vector
	float dx = x - (float)iX;
	float dy = y - (float)iY;

	// Compute the dot-product
	return (dx * gradient.x + dy * gradient.y);
}

float XorShift::Interpolate(float a0, float a1, float weight)
{
	// You may want clamping by inserting:
	//if (0.0 > weight) return a0;
	//if (1.0 < weight) return a1;

	return (a1 - a0) * weight + a0;
}

float XorShift::Perlin(float x, float y, unsigned int seedOverride)
{
	// Determine grid cell coordinates
	int x0 = (int)floor(x);
	int x1 = x0 + 1;
	int y0 = (int)floor(y);
	int y1 = y0 + 1;

	const Vec2 lerpWeight =
	{
		SmootherStep(x - (float)x0),
		SmootherStep(y - (float)y0)
	};

	// Lerp between grid point gradients
	float n0, n1;

	n0 = DotGridGradient(x0, y0, x, y, seedOverride);
	n1 = DotGridGradient(x1, y0, x, y, seedOverride);
	const float blendX0 = Lerp(n0, n1, lerpWeight.x);

	n0 = DotGridGradient(x0, y1, x, y, seedOverride);
	n1 = DotGridGradient(x1, y1, x, y, seedOverride);
	const float blendX1 = Lerp(n0, n1, lerpWeight.x);

	const float baseNoise = Lerp(blendX0, blendX1, lerpWeight.y);
	const float normalizedResult = ((baseNoise - (-TwoDSqrt2Over4)) / (( TwoDSqrt2Over4) - (-TwoDSqrt2Over4)));

	return normalizedResult;
}

float XorShift::NormalizeNoise(unsigned int noise)
{
	return static_cast<float>(noise) / static_cast<float>(UINT_MAX);
}

float XorShift::RandomNormalizedValue()
{
	return NormalizeNoise(Get1DNoise(RandomInRange(0, 100), static_cast<unsigned int>(m_State[2])));
}

void XorShift::ChangeSeedRandomly()
{
	SetSeed(Random());
}
