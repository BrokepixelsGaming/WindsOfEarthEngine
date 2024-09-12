#pragma once

#include <concepts>
#include <cmath>

template <typename T>
concept FloatType = std::is_floating_point_v<T>;

constexpr double kDoublePI = 3.141592653589793;

template<typename type>
type Square(type value)
{
	return value * value;
}

template<typename type>
type Cubed(type value)
{
	return value * value * value;
}

template<class FloatType>
constexpr FloatType Lerp(FloatType start, FloatType end, FloatType weight)
{
	// This function is generally based on std::lerp() from c++ 20, though it doesn't conform exactly:
	// https://en.cppreference.com/w/cpp/numeric/lerp
	return start + (weight * (end - start));
}

constexpr float SmoothStep(float normalizedValue)
{
	// Classic SmoothStep, implemeneted from the following article:
	// https://en.wikipedia.org/wiki/Smoothstep
	if (normalizedValue <= 0)
		return 0;
	else if (normalizedValue >= 1.f)
		return 1.f;
	else
		return (3.f * Square(normalizedValue)) - (2.f * Cubed(normalizedValue)); // 3x^2 - 2x^3
}

constexpr float SmootherStep(float normalizedValue)
{
	// This is the "smootherstep" function suggested by Ken Perlin and implemeneted from the following article:
	// https://en.wikipedia.org/wiki/Smoothstep
	if (normalizedValue <= 0)
		return 0;
	else if (normalizedValue >= 1.f)
		return 1.f;
	else
		return normalizedValue * normalizedValue * normalizedValue * (normalizedValue * (normalizedValue * 6.f - 15.f) + 10.f); // 6x^5 - 15x^4 + 10x^3
}

//constexpr float InverseSmoothStep(float normalizedValue)
//{
//	// This is the "inverse smootherstep" function implemented from the following article:
//	// https://en.wikipedia.org.wiki.Smoothstep
//	return 0.5f - std::sinf(std::asinf(1.f - (2.f * normalizedValue)) / 3.f);
//}