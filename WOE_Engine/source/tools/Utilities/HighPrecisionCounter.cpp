// HighPrecisionCounter.cpp
#include "HighPrecisionCounter.h"

void HighPrecisionCounter::StartCounter()
{
    m_startTime = std::chrono::high_resolution_clock::now();
}

double HighPrecisionCounter::GetDeltaSinceStart()
{
    TimePoint endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> timeSpan = std::chrono::duration_cast<std::chrono::duration<double>>(endTime - m_startTime);
    return timeSpan.count() * 1000.0;  // convert to milliseconds before returning
}
