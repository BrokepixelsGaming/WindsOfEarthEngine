#include "SeededGenerator.h"

#ifdef _DEBUG
	#include <iostream>
#endif // _DEBUG
///////////////////////////////////////////////////////////////
// Generator constructor with a seed. In debug mode it will print a 
//	seed to the console to support debugging.
///////////////////////////////////////////////////////////////
SeededGenerator::SeededGenerator(unsigned int seed)
	: m_seed(seed)
{
#ifdef _DEBUG
	std::cout << "Generator Seed:: " << m_seed << "\n";
#endif
	m_engine.seed(seed);
}
///////////////////////////////////////////////////////////////
// Generator constructor without a seed. In debug mode it will print a 
//	seed to the console to support debugging. 
///////////////////////////////////////////////////////////////
SeededGenerator::SeededGenerator()

{
	std::random_device randomDevice;
	m_seed = randomDevice();

#ifdef _DEBUG
	std::cout << "Generator Seed:: " << m_seed << "\n";
#endif
	m_engine.seed(m_seed);
}

// Overloaded from template. 
float SeededGenerator::GenerateNumberBetween(float min, float max)
{
	std::uniform_real_distribution<float> distribution(min, max);
	return distribution(m_engine);
}

// overloaded from template. 
double SeededGenerator::GenerateNumberBetween(double min, double max)
{

	std::uniform_real_distribution<double> distribution(min, max);
	return distribution(m_engine);
}
// overloaded from template. 
long double SeededGenerator::GenerateNumberBetween(long double min, long double max)
{

	std::uniform_real_distribution<long double> distribution(min, max);
	return distribution(m_engine);
}
