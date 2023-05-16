#ifndef DEVICE_HPP
#define DEVICE_HPP

#include "WarpEngine/Common.hpp"
#include "WarpEngine/Engine/Window.hpp"

namespace WarpEngine
{
    struct SwapChainSupportDetails
    {
        VkSurfaceCapabilitiesKHR surfaceCapabilities;
        std::vector<VkSurfaceFormatKHR> surfaceFormats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    struct QueueFamily
    {
        uint32_t graphicsFamily;
        uint32_t presentFamily;
        bool graphicsHasValue = false;
        bool presentHasValue = false;
        bool isComplete() { return graphicsHasValue && presentHasValue; }
    };

    class Device
    {
        public:
        #ifndef WARP_DEBUG
            const bool enableValidationLayers = true;
        #else
            const bool enableValidationLayers = false;
        #endif

        Device (const Device&) = delete;
        Device& operator= (const Device&) = delete;

        Device(Window &window);
        ~Device();

        void createBuffer(VkDeviceSize deviceSize,VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryFlags,VkBuffer,VkDeviceMemory deviceMemory);
        VkCommandBuffer beginSingleTimeCommands();
        void endSingleTimeCommands(VkCommandBuffer commandBuffer);
        void copyBuffer(VkBuffer srcBuffer,VkBuffer dstBuffer,VkDeviceMemory deviceMemory);

        QueueFamily findQueueFamilies(VkPhysicalDevice physicalDevice);
        SwapChainSupportDetails querySwapchainSupport(VkPhysicalDevice physicalDevice);

        VkPhysicalDeviceProperties physicalDeviceProperties;
        
        VkCommandPool commandPool;
        VkDevice _device;
        VkSurfaceKHR surface;
        VkQueue graphicsQueue;
        VkQueue presentQueue;

        private:
        void createInstance();
        void setupDebugMessenger();
        void createSurface();
        void pickPhysicalDevice();
        void createLogicalDevice();
        void createCommandPool();
        void getVulkanVersion();


        bool isDeviceSuitable(VkPhysicalDevice physicalDevice);
        bool checkValidationLayerSupport();
        void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);
        void hasGLFWrequiredExtensions();
        bool checkDeviceExtensionSupport(VkPhysicalDevice physicalDevice);

        std::vector<const char*> getRequiredExtensions();

        VkInstance instance;
        VkDebugUtilsMessengerEXT debugMessenger;
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        Window window;
        
        const std::vector<const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"};
        const std::vector<const char*> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

        uint32_t gameVersion;
        int numDevices;
    };
}

#endif