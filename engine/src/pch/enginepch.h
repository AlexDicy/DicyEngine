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

#include "utils/Common.h"
#include "utils/Logging.h"
#include "utils/Profiler.h"
#include "events/Event.h"
#include "events/Dispatcher.h"
#include "input/Input.h"
#include "registries/EntityScriptRegistry.h"
#include "registries/ShaderRegistry.h"
#include "rendering/camera/Camera.h"

#include "physics/Physics.h"
#include "physics/shapes/BoxShape.h"
#include "physics/shapes/SphereShape.h"

#include "scene/components/Mesh.h"
#include "scene/components/PointLight.h"
#include "scene/components/RigidBody.h"
#include "scene/components/Script.h"
#include "scene/components/Transform.h"
#include "scene/Scene.h"
