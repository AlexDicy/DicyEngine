add_rules("mode.debug", "mode.release")

add_requires("spdlog v1.14.1")
add_requires("glfw 3.4")
add_requires("imgui v1.91.1-docking", {configs = {glfw_opengl3 = true}})
add_requires("glm 1.0.1")
add_requires("entt v3.13.2")
add_requires("assimp v5.4.3")


function find_jdk_path()
    local jdk_path = os.getenv("JAVA_HOME")
    if jdk_path and os.isdir(jdk_path) then
        return jdk_path
    end

    local java_exe = is_host("windows") and "java.exe" or "java"
    local path_separator = is_host("windows") and ";" or ":"
    local path_env = os.getenv("PATH")

    if not path_env then
        print("Warning: PATH environment variable not found.")
        return nil
    end

    for directory in path_env:gmatch("[^" .. path_separator .. "]+") do
        local java_path = path.join(directory, java_exe)
        if os.isfile(java_path) then
            local java_home = path.directory(directory)
            local include_path = path.join(java_home, "include")
            if os.isdir(include_path) then
                return java_home
            end
        end
    end

    print("Warning: JAVA_HOME not set")
    print("Warning: Java executable not found in PATH")
    return nil
end


target("Engine")
    set_kind("binary")
    set_languages("cxx20")
    set_rundir("$(projectdir)/Engine")

    local jdk_path = find_jdk_path()
    set_pcxxheader("Engine/src/pch/enginepch.h")
    add_files("Engine/src/**.cpp")
    add_headerfiles("Engine/src/**.h")
    add_includedirs(
        "Engine/src",
        "Engine/libs/glad/include",
        "Engine/libs/stb",
        path.join(jdk_path, "include"),
        path.join(jdk_path, "include", "win32"),
        path.join(jdk_path, "include", "linux")
    )

    add_packages("spdlog", "glfw", "imgui", "glm", "entt", "assimp")
    add_deps("glad", "stb")

    add_defines("DE_IS_ENGINE", "IMGUI_IMPL_OPENGL_LOADER_CUSTOM", "GLM_ENABLE_EXPERIMENTAL")
    add_defines("DE_JAVA_HOME=\"" .. jdk_path .. "\"")
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
