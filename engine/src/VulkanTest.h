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
    void mainLoop() const;
    void drawFrame() const;

    void createInstance();
    bool checkValidationLayerSupport() const;
    void pickPhysicalDevice();
    void createLogicalDevice();
    void createSurface();
    void createSwapChain();
    void createImageViews();
    void createRenderPass();
    void createGraphicsPipeline();
    void createFramebuffers();
    void createCommandPool();
    void createCommandBuffer();
    void createSyncObjects();

    void recordCommandBuffer(vk::CommandBuffer commandBuffer, unsigned int imageIndex) const;

    unsigned int rateDeviceSuitability(vk::PhysicalDevice device);

    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete() const {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    QueueFamilyIndices findQueueFamilies(vk::PhysicalDevice device) const;
    bool checkDeviceExtensionSupport(vk::PhysicalDevice device) const;

    struct SwapChainSupportDetails {
        vk::SurfaceCapabilitiesKHR capabilities;
        std::vector<vk::SurfaceFormatKHR> formats;
        std::vector<vk::PresentModeKHR> presentModes;
    };

    SwapChainSupportDetails querySwapChainSupport(vk::PhysicalDevice device) const;
    vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats) const;
    vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes) const;
    vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities) const;

    vk::ShaderModule createShaderModule(const std::vector<char>& code) const;

    static std::vector<char> readFile(const std::string& filename);

    GLFWwindow* window = nullptr;
    vk::Instance instance;
    vk::PhysicalDevice physicalDevice = nullptr;
    vk::Device device;
    vk::Queue graphicsQueue;
    vk::Queue presentQueue;
    vk::SurfaceKHR surface;
    vk::SwapchainKHR swapChain;
    std::vector<vk::Image> swapChainImages;
    vk::Format swapChainImageFormat = {};
    vk::Extent2D swapChainExtent;
    std::vector<vk::ImageView> swapChainImageViews;
    vk::RenderPass renderPass;
    vk::PipelineLayout pipelineLayout;
    vk::Pipeline graphicsPipeline;
    std::vector<vk::Framebuffer> swapChainFramebuffers;
    vk::CommandPool commandPool;
    vk::CommandBuffer commandBuffer;

    vk::Semaphore imageAvailableSemaphore;
    vk::Semaphore renderFinishedSemaphore;
    vk::Fence inFlightFence;

    const std::vector<const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"};
    const std::vector<const char*> deviceExtensions = {vk::KHRSwapchainExtensionName};

#if defined(DE_DEBUG) && (!defined(DE_PLATFORM_WINDOWS) || defined(VK_ADD_LAYER_PATH))
    const bool enableValidationLayers = true;
#else
    const bool enableValidationLayers = false;
#endif
};
