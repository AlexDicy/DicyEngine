add_rules("mode.debug", "mode.release")

add_requires("spdlog v1.14.1")
add_requires("glfw 3.4")
add_requires("imgui v1.91.1-docking", {configs = {glfw_opengl3 = true}})
add_requires("glm 1.0.1")
add_requires("entt v3.13.2")
add_requires("assimp v5.4.3")


target("Engine")
    set_kind("binary")
    set_languages("cxx23")
    set_rundir("$(projectdir)/Engine")

    set_pcxxheader("Engine/src/pch/enginepch.h")
    add_files("Engine/src/**.cpp")
    add_headerfiles("Engine/src/**.h")
    add_includedirs(
        "Engine/src",
        "Engine/libs/glad/include",
        "Engine/libs/stb"
    )

    add_packages("spdlog", "glfw", "imgui", "glm", "entt", "assimp")
    add_deps("glad", "stb")

    add_defines("DE_IS_ENGINE", "IMGUI_IMPL_OPENGL_LOADER_CUSTOM", "GLM_ENABLE_EXPERIMENTAL")
    if is_mode("debug") then
        add_defines("DE_DEBUG")
    else
        add_defines("DE_RELEASE")
    end

    if is_plat("windows") then
        add_defines("DE_PLATFORM_WINDOWS", "OPENGL_4_6")
    elseif is_plat("macosx") then
        add_defines("DE_PLATFORM_MACOS", "OPENGL_4_1")
    end


-- Dependencies
target("glad")
    set_kind("static")
    set_languages("c17")
    set_runtimes("static")

    add_files("Engine/libs/glad/src/gl.c")
    add_includedirs("Engine/libs/glad/include")


target("stb")
    set_kind("static")
    set_languages("cxx17")
    set_runtimes("static")

    add_files("Engine/libs/stb/**.cpp")
    add_includedirs("Engine/libs/stb")
