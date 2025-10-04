#pragma once

enum class CursorMode {
    NORMAL = 0,
    HIDDEN = 1,
    DISABLED = 2
};

class Window {
public:
    virtual ~Window() = default;
    virtual void destroy() = 0;
    virtual void pollEvents() = 0;
    virtual void update() = 0;
    virtual unsigned int getWidth() const = 0;
    virtual unsigned int getHeight() const = 0;
    virtual int getFramebufferHeight() const = 0;
    virtual int getFramebufferWidth() const = 0;
    virtual bool isVSync() const = 0;
    virtual void setVSync(bool vsync) = 0;
    virtual float getLastFrameTime() const = 0;
    virtual float getScalingFactor() const = 0;
    virtual int getMonitorRefreshRate() const = 0;
    virtual void setCursorMode(CursorMode mode) const = 0;
    virtual void setRawMouseMotion(bool enabled) const = 0;

    static Ref<Window> create(const char* title, unsigned int width, unsigned int height);


#if defined(DE_PLATFORM_WINDOWS) || defined(DE_PLATFORM_MACOS)
    typedef struct GLFWwindow GLFWwindow; // NOLINT, do not include 'GLFW' here, it would interfere with 'glad'
    virtual GLFWwindow* getNativeWindow() const = 0;
#else
    #error "Unsupported platform"
#endif
};
