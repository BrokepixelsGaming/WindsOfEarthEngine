// Random.cpp
#include "Random.h"
#include <stdlib.h>
#include <time.h>

namespace Random {

    void Seed()
    {
        srand((unsigned int)time(nullptr));
    }

    float FRand()
    {
        return (float)rand() / (float)RAND_MAX;
    }

    float SignedFRand()
    {
        int initialRoll = rand();
        float halfRandMax = (float)RAND_MAX / 2.f;

        float result = ((float)initialRoll - halfRandMax) / halfRandMax;
        return result;
    }

    int RandomRange(int min, int max)
    {
        int range = max - min;
        int result = (rand() % range) + min;
        return result;
    }

}  // end Random
