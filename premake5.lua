workspace "DicyEngine"
    architecture "x64"
   
    configurations {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Engine"
    location "Engine"
    kind "SharedLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin/int/" .. outputdir .. "/%{prj.name}")

    pchheader "pch/enginepch.h"
    pchsource "Engine/src/pch/enginepch.cpp"

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs {
        "%{prj.name}/src",
        "%{prj.name}/libs/spdlog/include",
        "%{prj.name}/libs/GLFW/include"
    }

    links {
        "GLFW",
        "opengl32.lib"
    }

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "on"
        systemversion "latest"

        defines {
            "DE_PLATFORM_WINDOWS",
            "DE_IS_ENGINE"
        }

        postbuildcommands {
            -- create directory if it doesn't exist
            ("{MKDIR} ../bin/" .. outputdir .. "/Sandbox"),
            -- copy
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
        }

    filter "configurations:Debug"
        defines "DE_DEBUG"
        symbols "on"

    filter "configurations:Release"
        defines "DE_RELEASE"
        optimize "on"

    filter "configurations:Dist"
        defines "DE_DIST"
        optimize "on"


project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin/int/" .. outputdir .. "/%{prj.name}")

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs {
        "Engine/src",
        "Engine/libs/spdlog/include"
    }

    links {
        "Engine"
    }

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "on"
        systemversion "latest"

        defines {
            "DE_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "DE_DEBUG"
        symbols "on"

    filter "configurations:Release"
        defines "DE_RELEASE"
        optimize "on"

    filter "configurations:Dist"
        defines "DE_DIST"
        optimize "on"


project "GLFW"
    location "Engine/libs/GLFW"
    kind "StaticLib"
    language "C"
    staticruntime "on"
    warnings "off"

    targetdir ("Engine/libs/GLFW/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("Engine/libs/GLFW/bin/int/" .. outputdir .. "/%{prj.name}")

    files {
        "Engine/libs/GLFW/include/GLFW/glfw3.h",
        "Engine/libs/GLFW/include/GLFW/glfw3native.h",
        "Engine/libs/GLFW/src/glfw_config.h",
        "Engine/libs/GLFW/src/context.c",
        "Engine/libs/GLFW/src/init.c",
        "Engine/libs/GLFW/src/input.c",
        "Engine/libs/GLFW/src/monitor.c",
        
        "Engine/libs/GLFW/src/null_init.c",
        "Engine/libs/GLFW/src/null_joystick.c",
        "Engine/libs/GLFW/src/null_monitor.c",
        "Engine/libs/GLFW/src/null_window.c",
        
        "Engine/libs/GLFW/src/platform.c",
        "Engine/libs/GLFW/src/vulkan.c",
        "Engine/libs/GLFW/src/window.c"
    }

    filter "system:windows"
        files { 
            "Engine/libs/GLFW/src/win32_init.c",
            "Engine/libs/GLFW/src/win32_joystick.c",
            "Engine/libs/GLFW/src/win32_module.c",
            "Engine/libs/GLFW/src/win32_monitor.c",
            "Engine/libs/GLFW/src/win32_time.c",
            "Engine/libs/GLFW/src/win32_thread.c",
            "Engine/libs/GLFW/src/win32_window.c",
            "Engine/libs/GLFW/src/wgl_context.c",
            "Engine/libs/GLFW/src/egl_context.c",
            "Engine/libs/GLFW/src/osmesa_context.c"
        }

        defines { 
            "_GLFW_WIN32",
            "_CRT_SECURE_NO_WARNINGS"
        }

    filter "system:linux"
        pic "on"
   
        systemversion "latest"

        files {
            "Engine/libs/GLFW/src/src/x11_init.c",
            "Engine/libs/GLFW/src/src/x11_monitor.c",
            "Engine/libs/GLFW/src/src/x11_window.c",
            "Engine/libs/GLFW/src/src/xkb_unicode.c",
            "Engine/libs/GLFW/src/src/posix_module.c",
            "Engine/libs/GLFW/src/src/posix_time.c",
            "Engine/libs/GLFW/src/src/posix_thread.c",
            "Engine/libs/GLFW/src/src/posix_module.c",
            "Engine/libs/GLFW/src/src/glx_context.c",
            "Engine/libs/GLFW/src/src/egl_context.c",
            "Engine/libs/GLFW/src/src/osmesa_context.c",
            "Engine/libs/GLFW/src/src/linux_joystick.c"
        }
   
        defines {
            "_GLFW_X11"
        }

    filter "system:macosx"
        pic "on"
    
        files {
            "Engine/libs/GLFW/src/cocoa_init.m",
            "Engine/libs/GLFW/src/cocoa_monitor.m",
            "Engine/libs/GLFW/src/cocoa_window.m",
            "Engine/libs/GLFW/src/cocoa_joystick.m",
            "Engine/libs/GLFW/src/cocoa_time.c",
            "Engine/libs/GLFW/src/nsgl_context.m",
            "Engine/libs/GLFW/src/posix_thread.c",
            "Engine/libs/GLFW/src/posix_module.c",
            "Engine/libs/GLFW/src/osmesa_context.c",
            "Engine/libs/GLFW/src/egl_context.c"
        }

        defines {
            "_GLFW_COCOA"
        }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"
    
    filter { "system:windows", "configurations:Debug-AS" }    
        runtime "Debug"
        symbols "on"
        sanitize { "Address" }
        flags { "NoRuntimeChecks", "NoIncrementalLink" }
    
    filter "configurations:Release"
        runtime "Release"
        optimize "speed"
    
    filter "configurations:Dist"
        runtime "Release"
        optimize "speed"
        symbols "off"
