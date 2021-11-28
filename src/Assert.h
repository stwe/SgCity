#pragma once

#include "Log.h"

#if defined(_WIN64) && defined(_MSC_VER)
    #define SG_CITY_DEBUG_BREAK __debugbreak()
#elif defined(__linux__) && defined(__GNUC__) && (__GNUC__ >= 7)
    #include <csignal>
    #define SG_CITY_DEBUG_BREAK raise(SIGTRAP)
#else
    #error Unsupported platform or compiler!
#endif

#ifdef SG_CITY_DEBUG_BUILD
    #define SG_CITY_ENABLE_ASSERTS
#endif

#ifdef SG_CITY_ENABLE_ASSERTS
    #define SG_ASSERT(x, ...) { if(!(x)) { sg::Log::SG_LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); SG_CITY_DEBUG_BREAK; } }
#else
    #define SG_ASSERT(x, ...)
#endif
