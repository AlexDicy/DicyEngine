#pragma once
#ifdef OPENGL_4_6
#include <glad/gl.h>

class DebugGroup {
public:
    explicit DebugGroup(const std::string_view name) {
        glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, globalGroupDepth++, static_cast<int>(name.size()), name.data());
    }

    ~DebugGroup() {
        glPopDebugGroup();
        globalGroupDepth--;
    }

private:
    inline static unsigned int globalGroupDepth = 0;
};

#else

class DebugGroup {
public:
    explicit DebugGroup(const std::string_view) {}
};

#endif
