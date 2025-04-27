#pragma once
#include <Window.h>
#include "input/InputCodes.h"
#include "rendering/GraphicContext.h"

struct GLFWwindow; // do not include 'GLFW' here, it would interfere with 'glad'

class GLFW3Window final : public Window {
    GraphicContext *graphicCtx;
    GLFWwindow *window;
    bool vsync = false;
    float lastTime = 0.0;
    float lastFrameTime = 0.0;

public:
    GLFW3Window(const char *title, unsigned int width, unsigned int height);
    void destroy() override;
    void pollEvents() override;
    void update() override;
    unsigned int getWidth() const override;
    unsigned int getHeight() const override;
    int getFramebufferWidth() const override;
    int getFramebufferHeight() const override;
    bool isVSync() const override;
    void setVSync(bool vsync) override;
    float getLastFrameTime() const override;
    float getScalingFactor() const override;
    int getMonitorRefreshRate() const override;

    GLFWwindow *getNativeWindow() const override {
        return this->window;
    }

private:
    void registerEvents() const;
};

InputCode convertKeyToInputCode(int key);
InputCode convertMouseButtonToInputCode(int button);
