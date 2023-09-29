#pragma once
#include "common.h"

class DE_API Window {
public:
    virtual ~Window() = default;
    virtual void destroy() = 0;
    virtual void update() = 0;
    virtual unsigned int get_width() const = 0;
    virtual unsigned int get_height() const = 0;
    virtual bool is_vsync() const = 0;
    virtual void set_vsync(bool vsync) = 0;
    virtual double get_last_frame_time() const = 0;

    static std::shared_ptr<Window> create(const char *title, unsigned int width, unsigned int height);


#ifdef DE_PLATFORM_WINDOWS
    typedef struct GLFWwindow GLFWwindow; // do not include 'GLFW' here, it would interfere with 'glad'
    virtual GLFWwindow *get_native_window() const = 0;
#else
    #error "Unsupported platform"
#endif
};
