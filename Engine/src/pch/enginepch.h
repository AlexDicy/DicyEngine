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

// compile GLM with a left-handed coordinate system
#define GLM_FORCE_LEFT_HANDED

template<typename T>
using Ref = std::shared_ptr<T>;

#include "utils/logging.h"
#include "utils/profiler.h"
#include "events/event.h"
#include "events/dispatcher.h"
#include "input/input.h"
#include "rendering/shader_registry.h"
#include "rendering/camera/camera.h"
#include "scene/components/mesh.h"
#include "scene/components/transform.h"
