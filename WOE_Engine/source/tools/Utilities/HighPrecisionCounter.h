// HighPrecisionCounter.h
#pragma once

#include <chrono>

class HighPrecisionCounter
{
    typedef std::chrono::high_resolution_clock::time_point TimePoint;

    TimePoint m_startTime;

public:
    void StartCounter();
    double GetDeltaSinceStart();
};
