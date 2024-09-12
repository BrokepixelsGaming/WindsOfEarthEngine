#pragma once
#include <random>

///////////////////////////////////////////////////////////////
// A random number class. designed to be able to take a seed or 
//	generate a random seed. 
// Will take any intergral type and with a min / max set return a random
//	value between.  
///////////////////////////////////////////////////////////////
class SeededGenerator
{
private:

	unsigned int m_seed;
	std::mt19937 m_engine;

public:
	SeededGenerator(unsigned int seed);
	SeededGenerator();

	// Function to Generate a random number with overloads.  
	template<class Type>
	Type GenerateNumberBetween(Type min, Type max);
	float GenerateNumberBetween(float min, float max);
	double GenerateNumberBetween(double min, double max);
	long double GenerateNumberBetween(long double min, long double max);


};

// Template class in order to handle "Most" types. The specialisd types have overloads. 
template<class Type>
Type SeededGenerator::GenerateNumberBetween(Type min, Type max)
{
	std::uniform_int_distribution<Type> distribution(min, max);
	return distribution(m_engine);
}