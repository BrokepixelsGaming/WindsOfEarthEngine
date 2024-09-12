// Random.h
#pragma once

namespace Random
{
    void Seed();

    float FRand();  // returns a random number from 0 - 1, inclusive
    float SignedFRand();  // returns a random number from -1 to 1, inclusive
    int RandomRange(int min, int max);
}

