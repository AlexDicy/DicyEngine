#pragma once

#define _SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS
#include <iostream>
#undef _SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS
#include <functional>
#include <typeindex>

#include <string>
#include <vector>

#ifdef DE_PLATFORM_WINDOWS
    #include <Windows.h>
#endif

#include "common.h"
