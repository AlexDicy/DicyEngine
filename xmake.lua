add_rules("mode.debug", "mode.release")

add_repositories("dicy-xmake-registry https://github.com/Dicy/xmake-registry")

add_requires("spdlog v1.14.1")
add_requires("glfw 3.4")
add_requires("glm 1.0.1")
add_requires("entt v3.13.2")
add_requires("assimp v5.4.3")
add_requires("toml++ v3.4.0")
add_requires("chromium-embedded-framework 135.0.21")
add_requires("joltphysics v5.3.0")

target("engine")
    set_kind("binary")
    set_languages("cxx23")
    set_basename("DicyEngine")
    set_rundir("$(projectdir)/engine")

    add_files("engine/src/**.cpp")
    add_headerfiles("engine/src/**.h")
    add_includedirs(
        "engine/src",
        "engine/libs/glad/include",
        "engine/libs/stb"
    )
    on_load(function (target)
        import("xmake_utils")
        local jdk_path = xmake_utils.find_jdk_path()
        target:add("defines", "DE_JAVA_HOME=\"" .. jdk_path .. "\"")
        target:add("includedirs", path.join(jdk_path, "include"))

        if is_plat("windows") then
            target:add("includedirs", path.join(jdk_path, "include", "win32"))
        elseif is_plat("macosx") then
            target:add("includedirs", path.join(jdk_path, "include", "darwin"))
            target:add("links", path.join(jdk_path, "lib", "server", "libjvm.dylib"))
            target:add("rpathdirs", path.join(jdk_path, "lib", "server"))
        elseif is_plat("linux") then
            target:add("includedirs", path.join(jdk_path, "include", "linux"))
        end
    end)

    add_packages("spdlog", "glfw", "glm", "entt", "assimp", "toml++", "chromium-embedded-framework", "joltphysics")
    add_deps("glad", "stb")

    add_defines("DE_IS_ENGINE", "GLM_ENABLE_EXPERIMENTAL", "NDEBUG")
    if is_mode("debug") then
        add_defines("DE_DEBUG")
    else
        add_defines("DE_RELEASE")
    end

    if is_os("windows") then
        add_defines("DE_PLATFORM_WINDOWS", "OPENGL_4_6")
    elseif is_os("macosx") then
        add_defines("DE_PLATFORM_MACOS", "OPENGL_4_1")
    end

    if is_os("windows") then
        set_pcxxheader("engine/src/pch/enginepch.h")
        add_files("engine/windows/*.cpp")
        after_build(function (target)
            local cef_framework = path.join(target:pkg("chromium-embedded-framework"):installdir(), "bin", "*")
            os.cp(cef_framework, target:targetdir())
        end)
    elseif is_os("macosx") then
        set_pmxxheader("engine/src/pch/enginepch.h")
        add_rules("xcode.application")
        add_files("engine/macos/*.mm")
        add_files("engine/macos/Info.plist")
        after_build(function (target)
            local cef_framework = target:pkg("chromium-embedded-framework"):installdir() .. "/bin/Chromium Embedded Framework.framework"
            local destination = target:targetdir() ..  "/" .. target:basename() .. ".app/Contents/Frameworks/"
            os.cp(cef_framework, destination)
        end)
    end

-- CEF Helper bundles for macOS
if is_plat("macosx") then
    local cef_helper_configs = {
        {target_suffix = "",          name_suffix = "",            plist = "Helper-Info.plist"},
        {target_suffix = "-gpu",      name_suffix = " (GPU)",      plist = "Helper-GPU-Info.plist"},
        {target_suffix = "-plugin",   name_suffix = " (Plugin)",   plist = "Helper-Plugin-Info.plist"},
        {target_suffix = "-renderer", name_suffix = " (Renderer)", plist = "Helper-Renderer-Info.plist"}
    }
    for _, config in ipairs(cef_helper_configs) do
        local helper_output = "DicyEngine Helper" .. config.name_suffix
        target("engine-cef-helper" .. config.target_suffix)
            set_kind("binary")
            set_languages("cxx23")
            add_rules("xcode.application")
            set_targetdir("$(buildir)/$(plat)/$(arch)/$(mode)/DicyEngine.app/Contents/Frameworks")

            add_frameworks("AppKit", "Cocoa", "IOSurface")
            add_packages("chromium-embedded-framework")

            set_filename(helper_output)
            add_files("engine/macos/MacCEFProcessHelper.cpp")
            add_files("engine/macos/" .. config.plist)
            add_files(
                "engine/src/cef/OSRCefApp.cpp",
                "engine/src/cef/RendererMessageHandler.cpp"
            )
            add_headerfiles(
                "engine/src/cef/OSRCefApp.h",
                "engine/src/cef/RendererMessageHandler.h"
            )
            add_includedirs("engine/src/cef")

            after_build(function(target)
                local helper_dir = target:targetdir() ..  "/" .. helper_output .. ".app/Contents/"
                local source = path.join("engine/macos/", config.plist)
                local destination = path.join(helper_dir, "Info.plist")
                os.cp(source, destination)
            end)
    end
end

-- Dependencies
target("glad")
    set_kind("static")
    set_languages("c17")
    set_runtimes("static")

    add_files("engine/libs/glad/src/gl.c")
    add_includedirs("engine/libs/glad/include")


target("stb")
    set_kind("static")
    set_languages("cxx17")
    set_runtimes("static")

    add_files("engine/libs/stb/**.cpp")
    add_includedirs("engine/libs/stb")
