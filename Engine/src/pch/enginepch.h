// ReSharper disable CppClangTidyClangDiagnosticReservedMacroIdentifier, CppInconsistentNaming, CppClangTidyBugproneReservedIdentifier
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

#define GLM_FORCE_LEFT_HANDED

#include "logging.h"
#include "events/event.h"
#include "events/dispatcher.h"
