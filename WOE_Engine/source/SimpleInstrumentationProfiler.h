// SimpleInstrumentationProfiler.h
#pragma once

#include "tools/Utilities/HighPrecisionCounter.h"
#include <string>
#include <iostream>

class SimpleInstrumentationProfiler
{
    std::string m_label;
    HighPrecisionCounter m_counter;

public:
    SimpleInstrumentationProfiler(const char* label)
        : m_label(label)
    {
        m_counter.StartCounter();
    }

    ~SimpleInstrumentationProfiler()
    {
        double result = m_counter.GetDeltaSinceStart();
        std::cout << "Result for " << m_label << ": " << result << std::endl;
    }
};

// This enables the profiler.  Comment this line out if you want to disable the profiler.
#define _PROFILE

// Profiler macros
#ifdef _PROFILE
#define START_PROFILER(_str_) SimpleInstrumentationProfiler profiler(_str_)
#else
#define START_PROFILER(_str_)
#endif  // _PROFILE
