#include "pch/enginepch.h"
#include "VulkanTest.h"

int main() {
    VulkanTest app;

    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void VulkanTest::run() {
    initWindow();
    initVulkan();
    mainLoop();
    cleanup();
}

void VulkanTest::initWindow() {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(1920, 1080, "Vulkan", nullptr, nullptr);
}

void VulkanTest::initVulkan() {
    createInstance();
    createSurface();
    pickPhysicalDevice();
    createLogicalDevice();
}

void VulkanTest::mainLoop() {
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }
}

void VulkanTest::createInstance() {
    if (enableValidationLayers && !checkValidationLayerSupport()) {
        throw std::runtime_error("validation layers requested, but not available!");
    }

    vk::ApplicationInfo appInfo{};
    appInfo.sType = vk::StructureType::eApplicationInfo;
    appInfo.pApplicationName = "Hello Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    vk::InstanceCreateInfo createInfo{};
    createInfo.sType = vk::StructureType::eInstanceCreateInfo;
    createInfo.pApplicationInfo = &appInfo;

    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;
    if (enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    } else {
        createInfo.enabledLayerCount = 0;
    }

    if (vk::createInstance(&createInfo, nullptr, &instance) != vk::Result::eSuccess) {
        throw std::runtime_error("Failed to create instance!");
    }
}

bool VulkanTest::checkValidationLayerSupport() const {
#if defined(VK_ADD_LAYER_PATH) && defined(DE_PLATFORM_WINDOWS)
    _putenv_s("VK_ADD_LAYER_PATH", VK_ADD_LAYER_PATH);
#endif

    uint32_t layerCount;
    if (vk::enumerateInstanceLayerProperties(&layerCount, nullptr) != vk::Result::eSuccess) {
        throw std::runtime_error("Failed to enumerate instance layer properties!");
    }

    std::vector<vk::LayerProperties> availableLayers(layerCount);
    if (vk::enumerateInstanceLayerProperties(&layerCount, availableLayers.data()) != vk::Result::eSuccess) {
        throw std::runtime_error("Failed to enumerate instance layer properties!");
    }

    for (const char* layerName : validationLayers) {
        bool layerFound = false;

        for (const auto& layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }

        if (!layerFound) {
            return false;
        }
    }

    return true;
}

void VulkanTest::pickPhysicalDevice() {
    uint32_t deviceCount = 0;
    if (instance.enumeratePhysicalDevices(&deviceCount, nullptr) != vk::Result::eSuccess || deviceCount == 0) {
        throw std::runtime_error("Failed to find GPUs with Vulkan support!");
    }

    std::vector<vk::PhysicalDevice> devices(deviceCount);
    if (instance.enumeratePhysicalDevices(&deviceCount, devices.data()) != vk::Result::eSuccess) {
        throw std::runtime_error("Failed to enumerate physical devices!");
    }

    std::multimap<unsigned int, VkPhysicalDevice> candidates;
    for (const auto& device : devices) {
        unsigned int score = rateDeviceSuitability(device);
        candidates.insert(std::make_pair(score, device));
    }

    if (candidates.rbegin()->first > 0) {
        physicalDevice = candidates.rbegin()->second;
        std::cout << "Selected GPU: " << physicalDevice.getProperties().deviceName << std::endl;
    } else {
        throw std::runtime_error("Failed to find a suitable GPU!");
    }
}

void VulkanTest::createLogicalDevice() {
    QueueFamilyIndices indices = findQueueFamilies(physicalDevice);
    std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
    std::set uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    float queuePriority = 1.0f;
    for (unsigned int queueFamily : uniqueQueueFamilies) {
        vk::DeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = vk::StructureType::eDeviceQueueCreateInfo;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.emplace_back(queueCreateInfo);
    }

    vk::PhysicalDeviceFeatures deviceFeatures{};
    vk::DeviceCreateInfo createInfo{};
    createInfo.sType = vk::StructureType::eDeviceCreateInfo;
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = 0;
    if (physicalDevice.createDevice(&createInfo, nullptr, &device) != vk::Result::eSuccess) {
        throw std::runtime_error("Failed to create logical device!");
    }

    graphicsQueue = device.getQueue(indices.graphicsFamily.value(), 0);
    presentQueue = device.getQueue(indices.presentFamily.value(), 0);
}

void VulkanTest::createSurface() {
    if (glfwCreateWindowSurface(instance, window, nullptr, reinterpret_cast<VkSurfaceKHR*>(&surface)) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create window surface!");
    }
}

VulkanTest::QueueFamilyIndices VulkanTest::findQueueFamilies(vk::PhysicalDevice device) {
    QueueFamilyIndices indices;
    std::vector<vk::QueueFamilyProperties> queueFamilies = device.getQueueFamilyProperties();
    int i = 0;
    for (const auto& queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics) {
            indices.graphicsFamily = i;
        }
        vk::Bool32 presentSupport = false;
        if (device.getSurfaceSupportKHR(i, surface, &presentSupport) != vk::Result::eSuccess) {
            throw std::runtime_error("Failed to get device surface support!");
        }
        if (presentSupport) {
            indices.presentFamily = i;
        }
        if (indices.isComplete()) {
            break;
        }
        i++;
    }
    return indices;
}

unsigned int VulkanTest::rateDeviceSuitability(const vk::PhysicalDevice device) {
    const vk::PhysicalDeviceProperties properties = device.getProperties();
    const vk::PhysicalDeviceFeatures features = device.getFeatures();

    unsigned int score = 0;

    if (properties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu) {
        score += 1000;
    }

    score += properties.limits.maxImageDimension2D;

    if (!features.geometryShader) {
        return 0;
    }

    const QueueFamilyIndices indices = findQueueFamilies(device);
    if (!indices.isComplete()) {
        return 0;
    }

    if (indices.graphicsFamily == indices.presentFamily) {
        score += 1;
    }

    return score;
}

void VulkanTest::cleanup() const {
    device.destroy();
    instance.destroySurfaceKHR(surface);
    instance.destroy();
    glfwDestroyWindow(window);
    glfwTerminate();
}
