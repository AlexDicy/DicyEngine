#pragma once

class GraphicContext {
public:
    virtual ~GraphicContext() = default;
    virtual void init() = 0;
    virtual void swapBuffers() = 0;
};
