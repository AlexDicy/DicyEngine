#pragma once

class Window {
public:
    virtual ~Window() = default;
    virtual void destroy() = 0;
    virtual void update() = 0;
    virtual unsigned int getWidth() const = 0;
    virtual unsigned int getHeight() const = 0;
    virtual bool isVSync() const = 0;
    virtual void setVSync(bool vsync) = 0;
    virtual float getLastFrameTime() const = 0;

    static Ref<Window> create(const char* title, unsigned int width, unsigned int height);


#if defined(DE_PLATFORM_WINDOWS) || defined(DE_PLATFORM_MACOS)
    typedef struct GLFWwindow GLFWwindow; // NOLINT, do not include 'GLFW' here, it would interfere with 'glad'
    virtual GLFWwindow* getNativeWindow() const = 0;
#else
    #error "Unsupported platform"
#endif
};
