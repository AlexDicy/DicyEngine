workspace "DicyEngine"
    architecture "x64"
   
    configurations {
        "Debug",
        "Release",
        "Dist"
    }

    startproject "Engine"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"


project "Engine"
    location "Engine"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

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
        "%{prj.name}/libs/GLFW/include",
        "%{prj.name}/libs/glad/include",
        "%{prj.name}/libs/ImGui",
        "%{prj.name}/libs/glm",
        "%{prj.name}/libs/EnTT/single_include",
        "%{prj.name}/libs/stb",
    }

    links {
        "GLFW",
        "glad",
        "opengl32.lib",
        "ImGui",
        "stb"
    }

    defines {
        "DE_IS_ENGINE",
        "IMGUI_IMPL_OPENGL_LOADER_CUSTOM"
    }

    filter "system:windows"
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


group "Dependencies"
    project "GLFW"
        location "Engine/libs/GLFW"
        kind "StaticLib"
        language "C"
        warnings "off"
        staticruntime "on"

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
            systemversion "latest"

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


    project "glad"
        location "Engine/libs/glad"
        kind "StaticLib"
        language "C"
        warnings "off"
        staticruntime "on"

        targetdir ("Engine/libs/glad/bin/" .. outputdir .. "/%{prj.name}")
        objdir ("Engine/libs/glad/bin/int/" .. outputdir .. "/%{prj.name}")

        files {
            "Engine/libs/glad/include/glad/gl.h",
            "Engine/libs/glad/include/KHR/khrplatform.h",
            "Engine/libs/glad/src/gl.c"
        }

        includedirs {
            "Engine/libs/glad/include"
        }

        filter "system:windows"
            systemversion "latest"

        filter "configurations:Debug"
            runtime "Debug"
            symbols "on"
        
        filter "configurations:Release"
            runtime "Release"
            optimize "speed"
        
        filter "configurations:Dist"
            runtime "Release"
            optimize "speed"
            symbols "off"


    project "ImGui"
        location "Engine/libs/ImGui"
        kind "StaticLib"
        language "C++"
        cppdialect "C++17"
        warnings "off"
        staticruntime "on"

        targetdir ("Engine/libs/ImGui/bin/" .. outputdir .. "/%{prj.name}")
        objdir ("Engine/libs/ImGui/bin/int/" .. outputdir .. "/%{prj.name}")

        files {
            "Engine/libs/ImGui/imconfig.h",
            "Engine/libs/ImGui/imgui.h",
            "Engine/libs/ImGui/imgui.cpp",
            "Engine/libs/ImGui/imgui_draw.cpp",
            "Engine/libs/ImGui/imgui_internal.h",
            "Engine/libs/ImGui/imgui_tables.cpp",
            "Engine/libs/ImGui/imgui_widgets.cpp",
            "Engine/libs/ImGui/imstb_rectpack.h",
            "Engine/libs/ImGui/imstb_textedit.h",
            "Engine/libs/ImGui/imstb_truetype.h",
            "Engine/libs/ImGui/imgui_demo.cpp"
        }

        filter "system:windows"
            systemversion "latest"

        filter "system:linux"
            pic "On"
            systemversion "latest"

        filter "configurations:Debug"
            runtime "Debug"
            symbols "on"

        filter "configurations:Release"
            runtime "Release"
            optimize "on"

        filter "configurations:Dist"
            runtime "Release"
            optimize "on"
            symbols "off"


    project "stb"
        location "Engine/libs/stb"
        kind "StaticLib"
        language "C++"
        cppdialect "C++17"
        warnings "off"
        staticruntime "on"

        targetdir ("Engine/libs/stb/bin/" .. outputdir .. "/%{prj.name}")
        objdir ("Engine/libs/stb/bin/int/" .. outputdir .. "/%{prj.name}")

        files {
            "Engine/libs/stb/**.h",
            "Engine/libs/stb/**.cpp"
        }

        filter "system:windows"
            systemversion "latest"

        filter "system:linux"
            pic "On"
            systemversion "latest"

        filter "configurations:Debug"
            runtime "Debug"
            symbols "on"

        filter "configurations:Release"
            runtime "Release"
            optimize "on"

        filter "configurations:Dist"
            runtime "Release"
            optimize "on"
            symbols "off"

group ""
