#pragma once

#ifdef DE_PLATFORM_WINDOWS
    #define DE_API __declspec(dllexport)
#else
    #error "Unsupported platform"
#endif

#include "logging.h"
