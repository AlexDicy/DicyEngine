#pragma once
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

class VulkanTest {
public:
    void run();

private:
    void initWindow();
    void initVulkan();
    void cleanup() const;
    void mainLoop();

    void createInstance();
    bool checkValidationLayerSupport() const;
    void pickPhysicalDevice();
    void createLogicalDevice();
    void createSurface();

    unsigned int rateDeviceSuitability(vk::PhysicalDevice device);

    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete() const {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    QueueFamilyIndices findQueueFamilies(vk::PhysicalDevice device);

    GLFWwindow* window;
    vk::Instance instance;
    vk::PhysicalDevice physicalDevice = nullptr;
    vk::Device device;
    vk::Queue graphicsQueue;
    vk::Queue presentQueue;
    vk::SurfaceKHR surface;

    const std::vector<const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"};
    const std::vector<const char*> deviceExtensions = {vk::KHRSwapchainExtensionName};

#if defined(DE_DEBUG) && (!defined(DE_PLATFORM_WINDOWS) || defined(VK_ADD_LAYER_PATH))
    const bool enableValidationLayers = true;
#else
    const bool enableValidationLayers = false;
#endif
};
