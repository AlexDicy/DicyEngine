#include "pch/enginepch.h"
#include "GLFW3Window.h"
#include "events/Dispatcher.h"
#include "rendering/opengl/OpenGLContext.h"

#include <GLFW/glfw3.h>

Ref<Window> Window::create(const char* title, const unsigned int width, const unsigned int height) {
    return std::make_shared<GLFW3Window>(title, width, height);
}

GLFW3Window::GLFW3Window(const char* title, const unsigned int width, const unsigned int height) {
    if (!isGLFWInitialized) {
#ifdef DE_PLATFORM_MACOS
        glfwInitHint(GLFW_COCOA_CHDIR_RESOURCES, false);
#endif
        if (!glfwInit()) {
            DE_ERROR("Failed to initialize GLFW");
            return;
        }
        glfwSetErrorCallback([](int error, const char* description) {
            DE_ERROR("GLFW error [{0}] {1}", error, description);
        });
        isGLFWInitialized = true;
    }

#ifdef DE_PLATFORM_MACOS
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, true);
#else
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
#endif
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

    this->window = glfwCreateWindow(static_cast<int>(width), static_cast<int>(height), title, nullptr, nullptr);
    graphicCtx = new OpenGLContext(this->window);
    graphicCtx->init();

    this->GLFW3Window::setVSync(true);
    this->registerEvents();
}

void GLFW3Window::pollEvents() {
    glfwPollEvents();
}

void GLFW3Window::update() {
    this->graphicCtx->swapBuffers();

    const float currentTime = static_cast<float>(glfwGetTime());
    const float deltaTime = currentTime - this->lastTime;
    this->lastTime = currentTime;
    this->lastFrameTime = deltaTime;
}

void GLFW3Window::destroy() {
    glfwDestroyWindow(this->window);
}

unsigned int GLFW3Window::getWidth() const {
    int width, height;
    glfwGetWindowSize(this->window, &width, &height);
    return static_cast<unsigned int>(width);
}

unsigned int GLFW3Window::getHeight() const {
    int width, height;
    glfwGetWindowSize(this->window, &width, &height);
    return static_cast<unsigned int>(height);
}

int GLFW3Window::getFramebufferWidth() const {
    int width, height;
    glfwGetFramebufferSize(this->window, &width, &height);
    return width;
}

int GLFW3Window::getFramebufferHeight() const {
    int width, height;
    glfwGetFramebufferSize(this->window, &width, &height);
    return height;
}

bool GLFW3Window::isVSync() const {
    return this->vsync;
}

void GLFW3Window::setVSync(const bool vsync) {
    this->vsync = vsync;
    glfwSwapInterval(this->vsync ? 1 : 0);
}

float GLFW3Window::getLastFrameTime() const {
    return this->lastFrameTime;
}

float GLFW3Window::getScalingFactor() const {
    float scaleX;
    float scaleY;
    glfwGetWindowContentScale(this->window, &scaleX, &scaleY);
    return scaleX;
}

int GLFW3Window::getMonitorRefreshRate() const {
    // TODO: this reports only the refresh rate of the primary monitor, not the one where the window is located
    int refreshRate = 0;
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    if (monitor) {
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        if (mode) {
            refreshRate = mode->refreshRate;
        }
    }
    return refreshRate;
}

void GLFW3Window::registerEvents() const {
    static auto eventDispatcher = EventDispatcher::get();
    glfwSetWindowSizeCallback(this->window, [](GLFWwindow*, int new_width, int new_height) {
        eventDispatcher->dispatch(WindowResizeEvent(static_cast<unsigned int>(new_width), static_cast<unsigned int>(new_height)));
    });
    glfwSetFramebufferSizeCallback(this->window, [](GLFWwindow*, const int newWidth, const int newHeight) {
        eventDispatcher->dispatch(WindowFramebufferResizeEvent(newWidth, newHeight));
    });
    glfwSetWindowCloseCallback(this->window, [](GLFWwindow*) {
        eventDispatcher->dispatch(WindowCloseEvent());
    });
    glfwSetKeyCallback(this->window, [](GLFWwindow*, const int key, const int scancode, const int action, const int modifiers) {
        const KeyEventModifiers eventModifiers = {
            .ctrl = (modifiers & GLFW_MOD_CONTROL) != 0, //
            .shift = (modifiers & GLFW_MOD_SHIFT) != 0, //
            .alt = (modifiers & GLFW_MOD_ALT) != 0, //
            .super = (modifiers & GLFW_MOD_SUPER) != 0, //
        };
        const int hostKey = Input::getWindowsKeyCode(static_cast<InputCode>(key));
        switch (action) {
            case GLFW_PRESS:
                eventDispatcher->dispatch(KeyPressedEvent(convertKeyToInputCode(key), hostKey, scancode, eventModifiers, 0));
                break;
            case GLFW_REPEAT:
                eventDispatcher->dispatch(KeyPressedEvent(convertKeyToInputCode(key), hostKey, scancode, eventModifiers, 1));
                break;
            case GLFW_RELEASE:
                eventDispatcher->dispatch(KeyReleasedEvent(convertKeyToInputCode(key), hostKey, scancode, eventModifiers));
                break;
            default:
                break;
        }
    });
    glfwSetCharCallback(this->window, [](GLFWwindow*, const unsigned int character) {
        eventDispatcher->dispatch(CharTypedEvent(character));
    });
    glfwSetMouseButtonCallback(this->window, [](GLFWwindow* window, int button, int action, int) {
        double x;
        double y;
        glfwGetCursorPos(window, &x, &y);
        switch (action) {
            case GLFW_PRESS:
                eventDispatcher->dispatch(MouseButtonPressedEvent(static_cast<float>(x), static_cast<float>(y), convertMouseButtonToInputCode(button)));
                break;
            case GLFW_RELEASE:
                eventDispatcher->dispatch(MouseButtonReleasedEvent(static_cast<float>(x), static_cast<float>(y), convertMouseButtonToInputCode(button)));
                break;
            default:
                break;
        }
    });
    glfwSetScrollCallback(this->window, [](GLFWwindow* window, double offsetX, double offsetY) {
        double x;
        double y;
        glfwGetCursorPos(window, &x, &y);
        eventDispatcher->dispatch(MouseScrolledEvent(static_cast<float>(x), static_cast<float>(y), offsetX, offsetY));
    });
    glfwSetCursorPosCallback(this->window, [](GLFWwindow*, const double x, const double y) {
        eventDispatcher->dispatch(MouseMovedEvent(static_cast<float>(x), static_cast<float>(y)));
    });
}


InputCode convertKeyToInputCode(int key) {
    switch (key) {
            // @formatter:off
            // clang-format off
        case GLFW_KEY_SPACE: return InputCode::KEY_SPACE;
        case GLFW_KEY_APOSTROPHE: return InputCode::KEY_APOSTROPHE;
        case GLFW_KEY_COMMA: return InputCode::KEY_COMMA;
        case GLFW_KEY_MINUS: return InputCode::KEY_MINUS;
        case GLFW_KEY_PERIOD: return InputCode::KEY_PERIOD;
        case GLFW_KEY_SLASH: return InputCode::KEY_SLASH;
        case GLFW_KEY_0: return InputCode::KEY_0;
        case GLFW_KEY_1: return InputCode::KEY_1;
        case GLFW_KEY_2: return InputCode::KEY_2;
        case GLFW_KEY_3: return InputCode::KEY_3;
        case GLFW_KEY_4: return InputCode::KEY_4;
        case GLFW_KEY_5: return InputCode::KEY_5;
        case GLFW_KEY_6: return InputCode::KEY_6;
        case GLFW_KEY_7: return InputCode::KEY_7;
        case GLFW_KEY_8: return InputCode::KEY_8;
        case GLFW_KEY_9: return InputCode::KEY_9;
        case GLFW_KEY_SEMICOLON: return InputCode::KEY_SEMICOLON;
        case GLFW_KEY_EQUAL: return InputCode::KEY_EQUAL;
        case GLFW_KEY_A: return InputCode::KEY_A;
        case GLFW_KEY_B: return InputCode::KEY_B;
        case GLFW_KEY_C: return InputCode::KEY_C;
        case GLFW_KEY_D: return InputCode::KEY_D;
        case GLFW_KEY_E: return InputCode::KEY_E;
        case GLFW_KEY_F: return InputCode::KEY_F;
        case GLFW_KEY_G: return InputCode::KEY_G;
        case GLFW_KEY_H: return InputCode::KEY_H;
        case GLFW_KEY_I: return InputCode::KEY_I;
        case GLFW_KEY_J: return InputCode::KEY_J;
        case GLFW_KEY_K: return InputCode::KEY_K;
        case GLFW_KEY_L: return InputCode::KEY_L;
        case GLFW_KEY_M: return InputCode::KEY_M;
        case GLFW_KEY_N: return InputCode::KEY_N;
        case GLFW_KEY_O: return InputCode::KEY_O;
        case GLFW_KEY_P: return InputCode::KEY_P;
        case GLFW_KEY_Q: return InputCode::KEY_Q;
        case GLFW_KEY_R: return InputCode::KEY_R;
        case GLFW_KEY_S: return InputCode::KEY_S;
        case GLFW_KEY_T: return InputCode::KEY_T;
        case GLFW_KEY_U: return InputCode::KEY_U;
        case GLFW_KEY_V: return InputCode::KEY_V;
        case GLFW_KEY_W: return InputCode::KEY_W;
        case GLFW_KEY_X: return InputCode::KEY_X;
        case GLFW_KEY_Y: return InputCode::KEY_Y;
        case GLFW_KEY_Z: return InputCode::KEY_Z;
        case GLFW_KEY_LEFT_BRACKET: return InputCode::KEY_LEFT_BRACKET;
        case GLFW_KEY_BACKSLASH: return InputCode::KEY_BACKSLASH;
        case GLFW_KEY_RIGHT_BRACKET: return InputCode::KEY_RIGHT_BRACKET;
        case GLFW_KEY_GRAVE_ACCENT: return InputCode::KEY_GRAVE_ACCENT;
        case GLFW_KEY_WORLD_1: return InputCode::KEY_WORLD_1;
        case GLFW_KEY_WORLD_2: return InputCode::KEY_WORLD_2;
        case GLFW_KEY_ESCAPE: return InputCode::KEY_ESCAPE;
        case GLFW_KEY_ENTER: return InputCode::KEY_ENTER;
        case GLFW_KEY_TAB: return InputCode::KEY_TAB;
        case GLFW_KEY_BACKSPACE: return InputCode::KEY_BACKSPACE;
        case GLFW_KEY_INSERT: return InputCode::KEY_INSERT;
        case GLFW_KEY_DELETE: return InputCode::KEY_DELETE;
        case GLFW_KEY_RIGHT: return InputCode::KEY_RIGHT;
        case GLFW_KEY_LEFT: return InputCode::KEY_LEFT;
        case GLFW_KEY_DOWN: return InputCode::KEY_DOWN;
        case GLFW_KEY_UP: return InputCode::KEY_UP;
        case GLFW_KEY_PAGE_UP: return InputCode::KEY_PAGE_UP;
        case GLFW_KEY_PAGE_DOWN: return InputCode::KEY_PAGE_DOWN;
        case GLFW_KEY_HOME: return InputCode::KEY_HOME;
        case GLFW_KEY_END: return InputCode::KEY_END;
        case GLFW_KEY_CAPS_LOCK: return InputCode::KEY_CAPS_LOCK;
        case GLFW_KEY_SCROLL_LOCK: return InputCode::KEY_SCROLL_LOCK;
        case GLFW_KEY_NUM_LOCK: return InputCode::KEY_NUM_LOCK;
        case GLFW_KEY_PRINT_SCREEN: return InputCode::KEY_PRINT_SCREEN;
        case GLFW_KEY_PAUSE: return InputCode::KEY_PAUSE;
        case GLFW_KEY_F1: return InputCode::KEY_F1;
        case GLFW_KEY_F2: return InputCode::KEY_F2;
        case GLFW_KEY_F3: return InputCode::KEY_F3;
        case GLFW_KEY_F4: return InputCode::KEY_F4;
        case GLFW_KEY_F5: return InputCode::KEY_F5;
        case GLFW_KEY_F6: return InputCode::KEY_F6;
        case GLFW_KEY_F7: return InputCode::KEY_F7;
        case GLFW_KEY_F8: return InputCode::KEY_F8;
        case GLFW_KEY_F9: return InputCode::KEY_F9;
        case GLFW_KEY_F10: return InputCode::KEY_F10;
        case GLFW_KEY_F11: return InputCode::KEY_F11;
        case GLFW_KEY_F12: return InputCode::KEY_F12;
        case GLFW_KEY_F13: return InputCode::KEY_F13;
        case GLFW_KEY_F14: return InputCode::KEY_F14;
        case GLFW_KEY_F15: return InputCode::KEY_F15;
        case GLFW_KEY_F16: return InputCode::KEY_F16;
        case GLFW_KEY_F17: return InputCode::KEY_F17;
        case GLFW_KEY_F18: return InputCode::KEY_F18;
        case GLFW_KEY_F19: return InputCode::KEY_F19;
        case GLFW_KEY_F20: return InputCode::KEY_F20;
        case GLFW_KEY_F21: return InputCode::KEY_F21;
        case GLFW_KEY_F22: return InputCode::KEY_F22;
        case GLFW_KEY_F23: return InputCode::KEY_F23;
        case GLFW_KEY_F24: return InputCode::KEY_F24;
        case GLFW_KEY_F25: return InputCode::KEY_F25;
        case GLFW_KEY_KP_0: return InputCode::KEY_KP_0;
        case GLFW_KEY_KP_1: return InputCode::KEY_KP_1;
        case GLFW_KEY_KP_2: return InputCode::KEY_KP_2;
        case GLFW_KEY_KP_3: return InputCode::KEY_KP_3;
        case GLFW_KEY_KP_4: return InputCode::KEY_KP_4;
        case GLFW_KEY_KP_5: return InputCode::KEY_KP_5;
        case GLFW_KEY_KP_6: return InputCode::KEY_KP_6;
        case GLFW_KEY_KP_7: return InputCode::KEY_KP_7;
        case GLFW_KEY_KP_8: return InputCode::KEY_KP_8;
        case GLFW_KEY_KP_9: return InputCode::KEY_KP_9;
        case GLFW_KEY_KP_DECIMAL: return InputCode::KEY_KP_DECIMAL;
        case GLFW_KEY_KP_DIVIDE: return InputCode::KEY_KP_DIVIDE;
        case GLFW_KEY_KP_MULTIPLY: return InputCode::KEY_KP_MULTIPLY;
        case GLFW_KEY_KP_SUBTRACT: return InputCode::KEY_KP_SUBTRACT;
        case GLFW_KEY_KP_ADD: return InputCode::KEY_KP_ADD;
        case GLFW_KEY_KP_ENTER: return InputCode::KEY_KP_ENTER;
        case GLFW_KEY_KP_EQUAL: return InputCode::KEY_KP_EQUAL;
        case GLFW_KEY_LEFT_SHIFT: return InputCode::KEY_LEFT_SHIFT;
        case GLFW_KEY_LEFT_CONTROL: return InputCode::KEY_LEFT_CONTROL;
        case GLFW_KEY_LEFT_ALT: return InputCode::KEY_LEFT_ALT;
        case GLFW_KEY_LEFT_SUPER: return InputCode::KEY_LEFT_SUPER;
        case GLFW_KEY_RIGHT_SHIFT: return InputCode::KEY_RIGHT_SHIFT;
        case GLFW_KEY_RIGHT_CONTROL: return InputCode::KEY_RIGHT_CONTROL;
        case GLFW_KEY_RIGHT_ALT: return InputCode::KEY_RIGHT_ALT;
        case GLFW_KEY_RIGHT_SUPER: return InputCode::KEY_RIGHT_SUPER;
        case GLFW_KEY_MENU: return InputCode::KEY_MENU;
        default: return InputCode::INPUT_UNKNOWN;
            // clang-format on
            // @formatter:on
    }
}

InputCode convertMouseButtonToInputCode(int button) {
    switch (button) {
            // @formatter:off
            // clang-format off
        case GLFW_MOUSE_BUTTON_1: return InputCode::MOUSE_BUTTON_1;
        case GLFW_MOUSE_BUTTON_2: return InputCode::MOUSE_BUTTON_2;
        case GLFW_MOUSE_BUTTON_3: return InputCode::MOUSE_BUTTON_3;
        case GLFW_MOUSE_BUTTON_4: return InputCode::MOUSE_BUTTON_4;
        case GLFW_MOUSE_BUTTON_5: return InputCode::MOUSE_BUTTON_5;
        case GLFW_MOUSE_BUTTON_6: return InputCode::MOUSE_BUTTON_6;
        case GLFW_MOUSE_BUTTON_7: return InputCode::MOUSE_BUTTON_7;
        case GLFW_MOUSE_BUTTON_8: return InputCode::MOUSE_BUTTON_8;
        default: return InputCode::INPUT_UNKNOWN;
            // clang-format on
            // @formatter:on
    }
}