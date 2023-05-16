#include "WarpEngine/Engine/Device.hpp"

#include <set>
#include <unordered_set>

namespace WarpEngine
{
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
        void *pUserData)
    {
        std::cerr << "validation layer: " << pCallbackData->pMessage << "\n\n";

        return VK_FALSE;
    }

    VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
                                          const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
                                          const VkAllocationCallbacks *pAllocator,
                                          VkDebugUtilsMessengerEXT *pDebugMessenger)
    {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
        if (func != nullptr)
        {
            return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
        }
        else
        {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

    void DestroyDebugUtilsMessengerEXT(VkInstance instance,
                                       VkDebugUtilsMessengerEXT pDebugMessenger,
                                       const VkAllocationCallbacks *pAllocator)
    {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
        if (func != nullptr)
        {
            return func(instance, pDebugMessenger, pAllocator);
        }
    }

    Device::Device(Window &window) : window{window}
    {
        createInstance();
        setupDebugMessenger();
        createSurface();
        pickPhysicalDevice();
        createLogicalDevice();
        createCommandPool();
        getVulkanVersion();
    }

    Device::~Device()
    {
        vkDestroyCommandPool(_device, commandPool, nullptr);
        vkDestroyDevice(_device, nullptr);

        if (enableValidationLayers)
        {
            DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
        }

        vkDestroySurfaceKHR(instance, surface, nullptr);
        vkDestroyInstance(instance, nullptr);
    }

    QueueFamily Device::findQueueFamilies(VkPhysicalDevice physicalDevice)
    {
        QueueFamily indices;

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

        int i = 0;
        for (const auto &queueFamily : queueFamilies)
        {
            if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                indices.graphicsFamily = i;
                indices.graphicsHasValue = true;
            }
            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentSupport);
            if (queueFamily.queueCount > 0 && presentSupport)
            {
                indices.graphicsFamily = i;
                indices.graphicsHasValue = true;
            }
            if (indices.isComplete())
            {
                break;
            }
            i++;
        }
        return indices;
    }

    void Device::createInstance()
    {
        if (enableValidationLayers && !checkValidationLayerSupport())
        {
            throw std::runtime_error("ERROR 1: Validation layers requested, but not available!");
        }

        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "";
        appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 0);
        appInfo.pEngineName = "Warp Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);

        VkInstanceCreateInfo instanceCreateInfo{};
        instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instanceCreateInfo.pApplicationInfo = &appInfo;

        auto extensions = getRequiredExtensions();
        instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        instanceCreateInfo.ppEnabledExtensionNames = extensions.data();

        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
        if (enableValidationLayers)
        {
            instanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            instanceCreateInfo.ppEnabledLayerNames = validationLayers.data();

            populateDebugMessengerCreateInfo(debugCreateInfo);
            debugCreateInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debugCreateInfo;
        }
        else
        {
            instanceCreateInfo.enabledLayerCount = 0;
            instanceCreateInfo.pNext = nullptr;
        }

        if (vkCreateInstance(&instanceCreateInfo, nullptr, &instance) != VK_SUCCESS)
        {
            throw std::runtime_error("ERROR 2: Couldn't create instance!");
        }

        hasGLFWrequiredExtensions();
    }

    void Device::setupDebugMessenger()
    {
        if (!enableValidationLayers)
            return;

        VkDebugUtilsMessengerCreateInfoEXT createInfo;
        populateDebugMessengerCreateInfo(createInfo);
        if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger))
        {
            throw std::runtime_error("ERROR2 3: Couldn't create debug messenger!");
        }
    }

    void Device::createSurface()
    {
        window.createWindowSurface(instance, &surface);
    }

    void Device::pickPhysicalDevice()
    {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
        if (deviceCount == 0)
        {
            throw std::runtime_error("ERROR 5: No physical devices found!");
        }
        std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, physicalDevices.data());

        numDevices = deviceCount;

        for (auto device : physicalDevices)
        {
            if (isDeviceSuitable(device))
            {
                physicalDevice = device;
                return;
            };
        }
        if (physicalDevice == VK_NULL_HANDLE)
        {
            throw std::runtime_error("ERROR 6: No physical device suitable!");
        }

        vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProperties);
    }

    void Device::createLogicalDevice()
    {
    }

    void Device::createCommandPool()
    {
    }

    void Device::getVulkanVersion()
    {
        gameVersion = VK_VERSION_1_0;
        auto FN_vkEnumerateInstanceVersion = PFN_vkEnumerateInstanceVersion(vkGetInstanceProcAddr(nullptr, "vkEnumerateInstanceVersion"));
        if (vkEnumerateInstanceVersion)
        {
            vkEnumerateInstanceVersion(&gameVersion);
        }

        std::cout << "\n======= VULKAN INFO =======\n\n";

        std::cout << "API Version: " << VK_VERSION_MAJOR(gameVersion) << "." << VK_VERSION_MINOR(gameVersion) << "." << VK_VERSION_PATCH(gameVersion) << std::endl;

        uint32_t driverVersion = physicalDeviceProperties.driverVersion;
        std::cout << "Driver Version: " << VK_VERSION_MAJOR(driverVersion) << "." << VK_VERSION_MINOR(driverVersion) << "." << VK_VERSION_PATCH(driverVersion) << std::endl;

        std::cout << "Device count: " << numDevices << std::endl;

        std::cout << "Physical device: " << physicalDeviceProperties.deviceName << std::endl;

        std::cout << "Physical device ID: " << physicalDeviceProperties.deviceID << std::endl;

        switch (physicalDeviceProperties.deviceType)
        {
        case 0:
            std::cout << "Physical device Type: OTHER" << std::endl;
            break;
        case 1:
            std::cout << "Physical device Type: INTEGRATED" << std::endl;
            break;
        case 2:
            std::cout << "Physical device Type: DISCRETE" << std::endl;
            break;
        case 3:
            std::cout << "Physical device Type: VIRTUAL" << std::endl;
            break;
        case 4:
            std::cout << "Physical device Type: CPU" << std::endl;
            break;
        };

        std::cout << "Vendor ID: " << physicalDeviceProperties.vendorID << std::endl;

        std::cout << "\n============================\n\n";
    }

    bool WarpEngine::Device::isDeviceSuitable(VkPhysicalDevice physicalDevice)
    {
        QueueFamily indices = findQueueFamilies(physicalDevice);
    }
    bool Device::checkValidationLayerSupport()
    {
        return false;
    }
    void Device::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo)
    {
    }
    void Device::hasGLFWrequiredExtensions()
    {
    }
    bool Device::checkDeviceExtensionSupport(VkPhysicalDevice physicalDevice)
    {
        return false;
    }

    std::vector<const char *> WarpEngine::Device::getRequiredExtensions()
    {
        uint32_t glfwExtensionCount = 0;
        const char **glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

        if(enableValidationLayers){
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        return extensions;
    }
}