#include "pch/enginepch.h"
#include "common.h"

#include <GLFW/glfw3.h>

class DE_API Application {
public:
    Application() {
        logger::init();
        this->running = true;
    }

    void run() {
        glfwInit();
        const auto window = glfwCreateWindow(1280, 720, "DicyEngine", nullptr, nullptr);
        glfwMakeContextCurrent(window);
        while (this->running) {
            DE_TRACE("Running");
        }
    }
private:
    bool running;
};
