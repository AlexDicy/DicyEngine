#pragma once
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

class VulkanTest {
public:
    void run() {
        initWindow();
        initVulkan();
        mainLoop();
        cleanup();
    }

private:
    void initWindow();
    void initVulkan();
    void cleanup() const;
    void mainLoop();

    void createInstance();

    bool checkValidationLayerSupport() const;

    GLFWwindow* window;
    vk::Instance instance;

    const std::vector<const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"};

#ifdef DE_DEBUG
    const bool enableValidationLayers = true;
#else
    const bool enableValidationLayers = false;
#endif
};
