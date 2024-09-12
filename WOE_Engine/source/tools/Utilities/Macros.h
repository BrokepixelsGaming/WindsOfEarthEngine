// Macros.h
#pragma once

#include <type_traits>

#define SAFE_DELETE(_ptr_) delete _ptr_; _ptr_ = nullptr
#define SAFE_DELETE_ARRAY(_ptr_) delete[] _ptr_; _ptr_ = nullptr

template <class RetType = int, class FltType = float>
constexpr RetType RoundToNearest(FltType val)
{
    static_assert(std::is_integral<RetType>::value, "RetType template must be an integral type");
    static_assert(std::is_floating_point<FltType>::value, "FltType template must be an floating point type");
    return static_cast<RetType>(val + static_cast<FltType>((val >= 0 ? 0.5f : -0.5f)));
}

#define _LOGGING_ 0
#define THREADS 1
#define _FPS_COUNTER_ 0