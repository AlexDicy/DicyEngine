function find_jdk_path()
    local jdk_path = os.getenv("JAVA_HOME")
    if jdk_path and os.isdir(jdk_path) then
        return jdk_path
    end

    if is_host("macosx") and os.isexec("/usr/libexec/java_home") then
        local java_home = os.iorun("/usr/libexec/java_home --version 23"):trim()
        if java_home then
            return java_home
        end
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
